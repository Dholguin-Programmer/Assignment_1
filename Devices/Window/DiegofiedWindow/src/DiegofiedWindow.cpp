// System includes
#include <iostream>
#include <stdio.h>
#include <stdlib.h>

// 3rd Party includes
#include <glad/glad.h>
#include "GLFW/glfw3.h"

// Device includes
#include "DiegofiedWindow.h"

using namespace Window::Device;

namespace
{
    const char* LABEL = "DiegofiedWindow";
}

////////////////////////////////////////////////////////////////////////////////
Diegofied::Diegofied()
: Base(1500, 1024, "Diegofied Window", nullptr),
  p_renderer(new Renderer::SubComponent::Diegofied(*this)),
  m_currentObjectSelected(0)
{
    // do nothing
}

////////////////////////////////////////////////////////////////////////////////
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    Window::Device::Diegofied* device = reinterpret_cast<Window::Device::Diegofied*>(glfwGetWindowUserPointer(window));
    device->handleKeyInput(key, scancode, action, mods);
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::PrimeDevice()
{   
    // Attempt to initialzie the GLFW library
    bool noErrorsOccured = (GLFW_TRUE == glfwInit());

    if (!noErrorsOccured)
    {
        std::cerr << LABEL << " : Failed to initialize GLFW" << std::endl;
    }
    else
    {
        // Setup a static profile for now
        // I would probably try to set these values with more intelligence 
        // vs just setting some static values here. Probably tie this into our build
        // system where based upon whichever target we are building against, we set
        // these values accordingly. 
        glfwWindowHint(GLFW_SAMPLES, 4); // 4x antialiasing
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); 
        glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
        
        // To ignore window resizing corner cases (save time), just use full screen only
        m_monitorObj = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(m_monitorObj);
        
        glfwWindowHint(GLFW_RED_BITS, mode->redBits);
        glfwWindowHint(GLFW_GREEN_BITS, mode->greenBits);
        glfwWindowHint(GLFW_BLUE_BITS, mode->blueBits);
        glfwWindowHint(GLFW_REFRESH_RATE, mode->refreshRate);

        // Attempt to create our GLFW Window
        // TODO: Add support for whether we always want to go full screen or if we'd like a window mode
        // for now just ignore m_winHeight & m_winWidth
        m_windowObj = glfwCreateWindow(mode->width, mode->height, m_winName, m_monitorObj, NULL);
        m_winWidth = mode->width;
        m_winHeight = mode->height;

        // Store an instance of this class as the user defined window class for static OpenGL function handling
        glfwSetWindowUserPointer(m_windowObj, this);

        if ((m_windowObj == nullptr))
        {
            noErrorsOccured = false;
            std::cerr << LABEL << " : Failed to create GLFW Window" << std::endl;
            glfwTerminate();
        }
        else
        {
            glfwMakeContextCurrent(m_windowObj); // Initialize GLEW
            
            glfwSetKeyCallback(m_windowObj, key_callback);
            
            if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) 
            {
                noErrorsOccured = false;
                std::cerr << LABEL << " : Failed to initialize GLAD" << std::endl;
                glfwDestroyWindow(m_windowObj);
                glfwTerminate();
            }

            // Define the viewport dimensions
            glViewport(0, 0, m_winWidth, m_winHeight);

            if(!p_renderer->initSubComponent())
            {
                noErrorsOccured = false;
                std::cout << LABEL << " : Failed to init Renderer SubComponent" << std::endl;
                // Error reported in subcomponent
            }
        }
    }

    return noErrorsOccured;
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::StartDevice()
{   
    m_deviceRunning = true;
    p_renderer->drawToScreen(ExitPolicyNotSatisfied());
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::handleRefreshingWindow()
{
    glfwSwapBuffers(m_windowObj);
    glfwPollEvents();
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::shouldRendererStopDrawing()
{
    return !ExitPolicyNotSatisfied();
}

////////////////////////////////////////////////////////////////////////////////
int Diegofied::getWindowWidth()
{
    return m_winWidth;
}

////////////////////////////////////////////////////////////////////////////////
int Diegofied::getWindowHeight()
{
    return m_winHeight;
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::StopDevice()
{
    m_deviceRunning = false;
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::ExitPolicyNotSatisfied()
{
    // Here I'd also try to create either some sort of policy for our exit conditions,
    // as this might be something that differs across vendors... Just a thought

    bool exitPolicySatisfied = true;

    // These are our exit policies
    exitPolicySatisfied &= glfwWindowShouldClose(m_windowObj) == 0;  // Exit if flag was set
    exitPolicySatisfied &= m_deviceRunning;                          // Exit if device is not running

    // Exit if escape key is pressed
    exitPolicySatisfied &= (glfwGetKey(m_windowObj, GLFW_KEY_ESCAPE) == GLFW_PRESS) ? false : true;

    // TODO: Setup some sort of 'Key Observers' where our 'Key' would be Subjects (Observer/Subject Pattern)

    return exitPolicySatisfied;
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::HandleWindowResize(Window::Types::WindowObject* window, int width, int height)
{
    m_winWidth = width;
    m_winHeight = height;

    // Because we are only working from a full screen perspectiuve, we don't need to handle much here
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::handleKeyInput(int key, int scanecode, int action, int mods)
{
    // This isn't very flexible but 
    const static Renderer::Types::Vector3f unSelectedSize = p_renderer->m_dynamicRenderObjects[m_currentObjectSelected].m_scale;
    const Renderer::Types::Vector3f selectedColor = Renderer::Types::Colors::YELLOW;
    const static Renderer::Types::Vector3f unSelectedColor = p_renderer->m_dynamicRenderObjects[m_currentObjectSelected].m_colorType; 

    if (key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
    {
        if (m_currentObjectSelected < p_renderer->m_dynamicRenderObjects.size())
        {
            Renderer::SubComponent::Types::RenderObject* selectedObject = &p_renderer->m_dynamicRenderObjects[m_currentObjectSelected];
            float textXPos = selectedObject->m_position.x;
            float textYPos = selectedObject->m_position.y + 100.f;
            float textZPos = selectedObject->m_position.z;
            std::string mlbInfoString = selectedObject->m_mlbInfo.m_homeTeam + " Vs " + selectedObject->m_mlbInfo.m_awayTeam;
            std::string gameSeriesString = "Game " + selectedObject->m_mlbInfo.m_seriesGameNumber + " of series of " +
                                            selectedObject->m_mlbInfo.m_gamesInSeries; 

            // Change to selected object attributes (i.e Color change, and scale increase)
            selectedObject->m_colorType = selectedColor;
            selectedObject->m_scale = Renderer::Types::Vector3f(unSelectedSize * 1.5f); 

            // Update previous object whenever we aren't starting from the very beginning
            const bool updatePreviousObject = (m_currentObjectSelected != 0);
            if (updatePreviousObject)
            {
                // Change unselected object attributes (i.e Color change, and scale normalization)
                Renderer::SubComponent::Types::RenderObject* unselectedObject = &p_renderer->m_dynamicRenderObjects[m_currentObjectSelected - 1];
                unselectedObject->m_colorType = unSelectedColor;
                unselectedObject->m_scale = unSelectedSize;
            }

            // Remove any previous text objects, and populate a text object that will contain the current selected object's mlb info
            // See MlbInformation struct
            p_renderer->m_textToRender.clear();
            
            // Display Home vs Away team metadata
            p_renderer->createViewSpaceObject(mlbInfoString.c_str(),
                                              Renderer::Types::Colors::RED,
                                              Renderer::Types::Vector3f(selectedObject->m_position.x,
                                                                        selectedObject->m_position.y + 100.0f,
                                                                        selectedObject->m_position.x),
                                              Renderer::Types::Vector3f(0.35f,0.35f,1.0f),
                                              "shaders/vertex/font_text.vs",
                                              "shaders/vertex/font_text.fs");

            // Display Game metadata specific to which game in the series the home and away teams are playing
            p_renderer->createViewSpaceObject(gameSeriesString.c_str(),
                                              Renderer::Types::Colors::RED,
                                              Renderer::Types::Vector3f(selectedObject->m_position.x,
                                                                        selectedObject->m_position.y - 25.0f,
                                                                        selectedObject->m_position.x),
                                              Renderer::Types::Vector3f(0.35f,0.35f,1.0f),
                                              "shaders/vertex/font_text.vs",
                                              "shaders/vertex/font_text.fs");

            m_currentObjectSelected++; // We end on the index we have just changed to 'selected' (due to 0th index) 
        }
    }
    if (key == GLFW_KEY_LEFT && action == GLFW_PRESS)
    {
        if (m_currentObjectSelected > 1)
        {
            m_currentObjectSelected--; // We begin on the object we'll be changing to 'unselected'

            // Change unselected object attributes (i.e Color change, and scale normalization)
            Renderer::SubComponent::Types::RenderObject* unselectedObject = &p_renderer->m_dynamicRenderObjects[m_currentObjectSelected];
            unselectedObject->m_colorType = unSelectedColor;
            unselectedObject->m_scale = unSelectedSize;

            // Change to selected object attributes (i.e Color change, and scale increase)
            Renderer::SubComponent::Types::RenderObject* selectedObject = &p_renderer->m_dynamicRenderObjects[m_currentObjectSelected - 1];
            selectedObject->m_colorType = selectedColor;
            selectedObject->m_scale = Renderer::Types::Vector3f(unSelectedSize * 1.5f); 
            
            // Determine our text object's position with respect to the currently selected object
            float textXPos = selectedObject->m_position.x;
            float textYPos = selectedObject->m_position.y + 100.f;
            float textZPos = selectedObject->m_position.z;
            std::string awayHomeString = selectedObject->m_mlbInfo.m_homeTeam + " Vs " + selectedObject->m_mlbInfo.m_awayTeam;
            std::string gameSeriesString = "Game " + selectedObject->m_mlbInfo.m_seriesGameNumber + " of series of " +
                                            selectedObject->m_mlbInfo.m_gamesInSeries; 
            // Remove any previous text objects, and populate a text object that will contain the current selected object's mlb info
            // See MlbInformation struct
            p_renderer->m_textToRender.clear();
            
            // Display Home vs Away team metadata
            p_renderer->createViewSpaceObject(awayHomeString.c_str(),
                                              Renderer::Types::Colors::RED,
                                              Renderer::Types::Vector3f(selectedObject->m_position.x,
                                                                        selectedObject->m_position.y + 100.0f,
                                                                        selectedObject->m_position.x),
                                              Renderer::Types::Vector3f(0.35f,0.35f,1.0f),
                                              "shaders/vertex/font_text.vs",
                                              "shaders/vertex/font_text.fs");

            // Display Game metadata specific to which game in the series the home and away teams are playing
            p_renderer->createViewSpaceObject(gameSeriesString.c_str(),
                                              Renderer::Types::Colors::RED,
                                              Renderer::Types::Vector3f(selectedObject->m_position.x,
                                                                        selectedObject->m_position.y - 25.0f,
                                                                        selectedObject->m_position.x),
                                              Renderer::Types::Vector3f(0.35f,0.35f,1.0f),
                                              "shaders/vertex/font_text.vs",
                                              "shaders/vertex/font_text.fs");
        }
    }
}
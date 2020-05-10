#include <iostream>

// 3rd Party includes
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include <ft2build.h>
#include FT_FREETYPE_H
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/string_cast.hpp>

// SubComponent includes
#include "DiegofiedRenderer.h"

using namespace Renderer::SubComponent;

namespace
{
    const char* label = "DiegofiedRenderer";

    const unsigned int vertexPositionDimensions = 3;
    const unsigned int colorPositionDimensions = 3;
    const unsigned int texturePositionDimensions = 2;
}

////////////////////////////////////////////////////////////////////////////////
Diegofied::Diegofied(Window::Device::Manager& windowManager)
: Renderer::SubComponent::Base(windowManager)
{
    // do nothing
}

////////////////////////////////////////////////////////////////////////////////
Diegofied::~Diegofied()
{

}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::loadTexture(const char* texturePath, unsigned int& textureHandle)
{
    glGenTextures(1, &textureHandle);
    glBindTexture(GL_TEXTURE_2D, textureHandle);

    // Set Texture Wrapping properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);	
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);  

    // Set Texture Filtering properties
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    stbi_set_flip_vertically_on_load(true); // tell stb_image.h to flip loaded texture's on the y-axis.
    int width, height, nrChannels;
    unsigned char *data = stbi_load(texturePath, &width, &height, &nrChannels, 0);
    if (data)
    {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        std::cout << "Failed to load texture" << std::endl;
    }
    stbi_image_free(data);

    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::createViewSpaceObject(const char* text,
                                      Renderer::Types::Vector3f color,
                                      Renderer::Types::Vector3f position,
                                      Renderer::Types::Vector3f scale,
                                      const char* vertexShaderPath,
                                      const char* fragmentShaderPath)
{
    bool objectSuccessfullyCreated = true;
    if (isShaderSupported(vertexShaderPath) && isShaderSupported(fragmentShaderPath))
    {
        Types::TextObject textObject;
        textObject.m_text = text;
        textObject.m_colorType = color;
        textObject.m_scale = scale;
        textObject.m_position = position;
        textObject.m_vertexShader = vertexShaderPath;
        textObject.m_fragmentShader = fragmentShaderPath;

        // Let's handle generating our Text object 
        unsigned int VAO, VBO;
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);

        textObject.m_openGlBuffer.m_vertexArray = VAO;
        textObject.m_openGlBuffer.m_vertexBuffer = VBO;

        m_textToRender.push_back(textObject);

    }
    else
    {
        std::cerr << label << "::"<<  __FUNCTION__ << ": Doesn't support commanded shader type" << std::endl;      
    }

    return objectSuccessfullyCreated;
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::createGeometricObject(Renderer::Types::MlbInfo mlbInfo,
                                      Renderer::Types::GeometricObjects type,
                                      Renderer::Types::RenderingType renderType,
                                      Renderer::Types::Vector3f color,
                                      Renderer::Types::Textures textureType,
                                      const char* texturePath,
                                      Renderer::Types::Vector3f position,
                                      Renderer::Types::Vector3f scale,
                                      const char* vertexShaderPath,
                                      const char* fragmentShaderPath)
{
    /*
        Object Creation Step 1: Create the actual object and update it's
        fields. Be sure to make sure the type of object is one we support
    */

    bool objectSuccessfullyCreated = true;
    if (isShaderSupported(vertexShaderPath) && isShaderSupported(fragmentShaderPath))
    {
        // Populate struct with it's corressponding values
        Types::RenderObject newRenderObject;
        newRenderObject.m_mlbInfo = mlbInfo;
        newRenderObject.m_type = type;
        newRenderObject.m_scale = scale;
        newRenderObject.m_position = position;
        newRenderObject.m_openGlBuffer.m_numDimensions = 5;
        newRenderObject.m_vertexShader = vertexShaderPath;
        newRenderObject.m_fragmentShader = fragmentShaderPath;
        newRenderObject.m_textureUsed = (textureType == Renderer::Types::Textures::USING_TEXTURE);
        newRenderObject.m_colorType = color;
        newRenderObject.m_openGlBuffer.m_renderType = renderType; 

        // Update Texture related variables
        if (newRenderObject.m_textureUsed && isTextureSupported(texturePath))
        {
            // Load texture
            loadTexture(texturePath, newRenderObject.m_texture);
        }

        // Populate Buffer related data
        populatePrimitiveTypesVerticeData(newRenderObject);        
        populateOpenGlBuffers(newRenderObject);

        // Push our object on whether it is an object that is STATIC or DYNAMIC drawn (i.e changes alot or not)
        if (renderType == Renderer::Types::RenderingType::STATIC)
        {
            m_staticRenderObjects.push_back(newRenderObject);
        }
        else
        {
            m_dynamicRenderObjects.push_back(newRenderObject);
        }
    }
    else
    {
        std::cerr << label << "::"<<  __FUNCTION__ << ": Doesn't support commanded shader type" << std::endl;
    }
    
    return objectSuccessfullyCreated;
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::drawToScreen(bool exitCondition)
{
    bool stopDrawing = false;

    // Shader that is used whenever we are rendering any of our "GeometricObject" types
    Shader::SubComponent::Diegofied vertexColorTextureShader("shaders/vertex/3D_color_vertex_texture.vs", "shaders/fragment/3D_color_vertex_texture.fs"); 
    
    // Shader that is used whenever we are rendering any of our "TextObject" types
    Shader::SubComponent::Diegofied textShader("shaders/vertex/font_text.vs", "shaders/fragment/font_text.fs"); 

    // Set up our Projection Matrix (Since we're only caring about 2D in our case, just use a Orthographic Projection) 
    glm::mat4 Projection = glm::ortho( 0.0f, (float)m_windowManager.getWindowWidth(), 0.0f, (float)m_windowManager.getWindowHeight(),0.1f, 100.0f);
    
    // // Set up View Matrix (Being 2D in our case, just use the Identify Matrix (i.e ignore View Space as we're living in the simple 2D world and aren't moving our camera))
    glm::mat4 View = glm::mat4(1.0f);
    
    // Update any of our vertex & fragment shaders to point to the correct Diegofied Shader
    // TODO: In future, be able to add and reference these shaders via some UUID vs string literals
    m_vertexShaderLookupTable["shaders/vertex/3D_color_vertex_texture.vs"] = &vertexColorTextureShader;
    m_vertexShaderLookupTable["shaders/vertex/font_text.vs"] = &textShader;
    m_fragmentShaderLookupTable["shaders/fragment/3D_color_vertex_texture.fs"] = &vertexColorTextureShader;
    m_fragmentShaderLookupTable["shaders/fragment/font_text.fs"] = &textShader;

    while (!stopDrawing)
    {
        glClearColor(0.15f, 0.6f, 0.4f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        // Handle drawing any STATIC Geometric objects (i.e that'll actually manipulate it's Local Space)
        for(auto it = m_staticRenderObjects.begin(); it != m_staticRenderObjects.end(); it++)
        {
            // Enable OpenGL options
            glEnable(GL_DEPTH);

            // Use our shader specific for rendering text in our window
            Shader::SubComponent::Diegofied* shader = m_vertexShaderLookupTable[it->m_vertexShader];
            shader->use();

            if (it->m_textureUsed)
            {
                glBindTexture(GL_TEXTURE_2D, it->m_texture);
                shader->setBool("isTextureUsed", true);
                shader->setBool("isColorUsed", false);
            }
            else
            {
                shader->setBool("isColorUsed", true);       
                shader->setVec3("objectColor", it->m_colorType);
            }


            // Use the Vetex Array of our specific object, detailing it's position, color, and texture
            glBindVertexArray(it->m_openGlBuffer.m_vertexArray);

            // Update our Matrix representation of our Model's view based upon our specified position
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, it->m_position);
            Model = glm::scale(Model, it->m_scale);

            // pass transformation matrices to the shader program
            shader->setMat4("view", View);
            shader->setMat4("model", Model);
            shader->setMat4("projection", Projection);

            glBindVertexArray(it->m_openGlBuffer.m_vertexArray);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For now only support one type of geometric object (Rectangle as 2 Triangles hence 6 vertices)
            
            // Disable OpenGL options
            glDisable(GL_DEPTH);
        }

        // Handle drawing any DYNAMIC Geometric objects (i.e that'll actually manipulate it's Local Space)
        for(auto it = m_dynamicRenderObjects.begin(); it != m_dynamicRenderObjects.end(); it++)
        {
            // Enable OpenGL options
            glEnable(GL_DEPTH);

            // Use our shader specific for rendering text in our window
            Shader::SubComponent::Diegofied* shader = m_vertexShaderLookupTable[it->m_vertexShader];
            shader->use();

            if (it->m_textureUsed)
            {
                glBindTexture(GL_TEXTURE_2D, it->m_texture);
                shader->setBool("isTextureUsed", true);
                shader->setBool("isColorUsed", false);
            }
            else
            {
                shader->setBool("isColorUsed", true);       
                shader->setVec3("objectColor", it->m_colorType);
            }

            // Use the Vetex Array of our specific object, detailing it's position, color, and texture
            glBindVertexArray(it->m_openGlBuffer.m_vertexArray);

            // Update our Matrix representation of our Model's view based upon our specified position
            glm::mat4 Model = glm::mat4(1.0f);
            Model = glm::translate(Model, it->m_position);
            Model = glm::scale(Model, it->m_scale);

            // pass transformation matrices to the shader program
            shader->setMat4("view", View);
            shader->setMat4("model", Model);
            shader->setMat4("projection", Projection);

            glBindVertexArray(it->m_openGlBuffer.m_vertexArray);
            glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0); // For now only support one type of geometric object (Rectangle as 2 Triangles hence 6 vertices)
            
            // Disable OpenGL options
            glDisable(GL_DEPTH);
        }

        // Handle any text objects that currently only take place in the View space and doesn't use any Local Space operations
        for(auto it = m_textToRender.begin(); it != m_textToRender.end(); it++)
        {
            // Enable OpenGL options
            glEnable(GL_CULL_FACE);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

            // Use our shader specific for rendering text in our window
            Shader::SubComponent::Diegofied* shader = m_vertexShaderLookupTable[it->m_vertexShader];
            shader->use();
            shader->setMat4("projection", Projection);
            shader->setVec3("textColor", it->m_colorType);

            // Draw text to screen
            RenderText(textShader, *it);
            
            // Disable OpenGL options
            glDisable(GL_CULL_FACE);
            glDisable(GL_BLEND);
        }

        // We've finished drawing, let's flag to our window device, to handle any events that
        // fall outside of scope of this class
        m_windowManager.handleRefreshingWindow();
        stopDrawing = m_windowManager.shouldRendererStopDrawing();
    }
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::initSubComponent()
{
    /*  
        Important Note: Here I'm going to limit the type of characters we support to
        only support the first 128 characters of the ASCII character set. In the future,
        I'd probably pass in some sort of CharacterSet Policy which we'd tie into which
        fonts we, as a device, support.

        Note that our APP must provide these set of files until this CharacterSet Policy is
        further abstracted and integrated into this solution.
    */
    bool noErrorsOccured = true;

    // (1) Load and Initialize our Freetype library
    FT_Library ftLibraryHandle;
    if (FT_Init_FreeType(&ftLibraryHandle))
    {
        noErrorsOccured = false;
        std::cout << label << "::" << __FUNCTION__ << " : Could not init FreeType Library" << std::endl;
    }

    // (2) Load our TTF font type using our initalized Freetype library functionality
    FT_Face fontHandle;
    if (FT_New_Face(ftLibraryHandle, "fonts/Antaro.ttf", 0, &fontHandle))
    {
        noErrorsOccured = false;
        std::cout << label << "::" << __FUNCTION__ << " : Could not load Antaro TTF Font" << std::endl;
    }

    if (noErrorsOccured)
    {
        // set size to load glyphs as
        FT_Set_Pixel_Sizes(fontHandle, 0, 48);

        // disable byte-alignment restriction
        glPixelStorei(GL_UNPACK_ALIGNMENT, 1); 

        // Load first 128 characters of ASCII set
        for (unsigned char c = 0; c < 128 && noErrorsOccured; c++)
        {
            // Load character glyph 
            if (FT_Load_Char(fontHandle, c, FT_LOAD_RENDER))
            {
                noErrorsOccured = false;
                std::cout << label << "::" << __FUNCTION__ << " : Could not load Antaro TTF Glyph for character (ASCII): " << c << std::endl;
                continue;
            }

            // Generate our character texture
            unsigned int characterTexture;
            glGenTextures(1, &characterTexture);
            glBindTexture(GL_TEXTURE_2D, characterTexture);

            glTexImage2D(
                GL_TEXTURE_2D,
                0,
                GL_RED,
                fontHandle->glyph->bitmap.width,
                fontHandle->glyph->bitmap.rows,
                0,
                GL_RED,
                GL_UNSIGNED_BYTE,
                fontHandle->glyph->bitmap.buffer
            );

            // Set various texture attributes
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            
            // Store Texture and Character glyph related data in our character mapping struct 
            Types::Character character = 
            {
                characterTexture,
                glm::ivec2(fontHandle->glyph->bitmap.width, fontHandle->glyph->bitmap.rows),
                glm::ivec2(fontHandle->glyph->bitmap_left, fontHandle->glyph->bitmap_top),
                fontHandle->glyph->advance.x
            };
            
            // Store Texture and Character glyph related data in our character mapping struct 
            m_characterMapping.insert(std::pair<char, Types::Character>(c, character));
        }

        // Unbind our texture
        glBindTexture(GL_TEXTURE_2D, 0);
        
        // destroy FreeType once we're finished
        FT_Done_Face(fontHandle);
        FT_Done_FreeType(ftLibraryHandle);
    }

    return noErrorsOccured;
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::isGeometricObjectSupported(Renderer::Types::GeometricObjects type)
{
    /*
`       For now we'll just assume we support everything.. although maybe on other
        platforms, this might be based upon some sort of version of software/firmware/etc
    */
    return true;
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::isTextureSupported(const char* path)
{
    /*
`       For now we'll just assume we support everything.. although maybe on other
        platforms, this might be based upon some sort of version of software/firmware/etc
    */
    return true;  
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::isShaderSupported(const char* path)
{
    /* For now, we'll just assume that Diegofied Shaders can only work for this
       Renderer, but we might want to support more than just Diegofied Shaders

        i.e DeviceARendererShader is supported on DiegofiedRenderer
    */

    return true;
}

////////////////////////////////////////////////////////////////////////////////
void Diegofied::RenderText(Shader::SubComponent::Diegofied &shader, Types::TextObject obj)
{
    // Bind to our texture (currently using only one, so refer to only first texture unit)	
    glActiveTexture(GL_TEXTURE0);
    
    // Bind our VAO
    glBindVertexArray(obj.m_openGlBuffer.m_vertexArray);

    // Convert each character of the string we'll be rendering to our internal struct Types::Character
    // and setup texturizing the TTF's glyph into something that can be drawn in the shape of a QUAD 
    for (auto c = obj.m_text.begin(); c != obj.m_text.end(); c++) 
    {
        // Convert specific ASCII character to our internal 
        Types::Character ch = m_characterMapping[*c];

        // Calcualte x and y position of the specific character we will be drawing
        float xpos = obj.m_position.x + ch.Bearing.x * obj.m_scale.x;
        float ypos = obj.m_position.y - (ch.Size.y - ch.Bearing.y) * obj.m_scale.y;

        // Calculate the width and height of each character using glyph data
        float w = ch.Size.x * obj.m_scale.x;
        float h = ch.Size.y * obj.m_scale.y;
        
        // Our vertices will be a 4 Dimensional Vertice, with a total of 6 vertices (QUAD)
        float vertices[6][4] =
        {
            // Vertice 1 
            { xpos,     // X Object Coordinate Position
              ypos + h, // Y Object Coordinate Position
              0.0f,     // X Texture Coordinate Position
              0.0f      // Y Texture Coordinate Position 
            },
            // Vertice 2            
            { xpos,     // X Object Coordinate Position
              ypos,     // Y Object Coordinate Position
              0.0f,     // X Texture Coordinate Position
              1.0f      // Y Texture Coordinate Position
            },
            // Vertice 3
            { xpos + w, // X Object Coordinate Position
              ypos,     // Y Object Coordinate Position
              1.0f,     // X Texture Coordinate Position
              1.0f      // Y Texture Coordinate Position
            },
            // Vertice 4
            { xpos,     // X Object Coordinate Position
              ypos + h, // Y Object Coordinate Position
              0.0f,     // X Texture Coordinate Position
              0.0f      // Y Texture Coordinate Position
            },
            // Vertice 5
            { xpos + w, // X Object Coordinate Position
              ypos,     // Y Object Coordinate Position
              1.0f,     // X Texture Coordinate Position
              1.0f      // Y Texture Coordinate Position
            },
            // Vertice 6
            { xpos + w, // X Object Coordinate Position
              ypos + h, // Y Object Coordinate Position
              1.0f,     // X Texture Coordinate Position
              0.0f      // Y Texture Coordinate Position
            }           
        };
        
        // Bind to the texture of the specific character's texture
        glBindTexture(GL_TEXTURE_2D, ch.TextureID);

        // Bind to our VBO, and buffer add in our vertice data that will be used in our fragment/vertex shader programs
        glBindBuffer(GL_ARRAY_BUFFER, obj.m_openGlBuffer.m_vertexBuffer);

        // be sure to use glBufferSubData and not glBufferData
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

        // Bind our Array Buffer
        glBindBuffer(GL_ARRAY_BUFFER, 0);

        // Draw our QUAD
        glDrawArrays(GL_TRIANGLES, 0, 6);
        
        // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
        obj.m_position.x += (ch.Advance >> 6) * obj.m_scale.x; 
        // bitshift by 6 to get value in pixels (2^6 = 64 (divide amount of 1/64th pixels by 64 to get amount of pixels))
    }

    // Unbind our VAO
    glBindVertexArray(0);

    // Unbind our texture 
    glBindTexture(GL_TEXTURE_2D, 0);
}

////////////////////////////////////////////////////////////////////////////////
bool Diegofied::populatePrimitiveTypesVerticeData(Types::RenderObject& obj)
{
    bool conversionWasSuccess = true;

    /*
        Important Note: For time savings, we'll just always fetch for the current
        windows height and width, despite us knowing that it never changes. Possible
        enchancement would be to cache this value internally with an inline value as this
        value will always be referenced and doing this enchancement could save us CPU cycles.

        We'll just ignore this for now and just manually perform a call to our Window Manager
        class everytime we wish to create a object
    */

    const float xLowerBound = 0;
    const float xUpperBound = (float) m_windowManager.getWindowWidth();

    const float yLowerBound = 0;
    const float yUpperBound = (float) m_windowManager.getWindowHeight();

    // Clear buffers in case they are already populated
    obj.m_openGlBuffer.m_indices.clear();
    obj.m_openGlBuffer.m_vertices.clear();

    switch(obj.m_type)
    {
        case Renderer::Types::GeometricObjects::SQUARE:
        {
            ///////////////////////////////////////
            // Add Top Right (Indice Node 0)
            ///////////////////////////////////////

            // View Space Coordiantes (i.e Normal Device Coordinates with respect to our orthogonal view)
            obj.m_openGlBuffer.m_vertices.push_back(xUpperBound); // X Pos
            obj.m_openGlBuffer.m_vertices.push_back(yUpperBound); // Y Pos
            obj.m_openGlBuffer.m_vertices.push_back(0.0f);        // Z Pos

            if (obj.m_textureUsed)
            {
                // Texture Coordinates
                obj.m_openGlBuffer.m_vertices.push_back(1.0f);
                obj.m_openGlBuffer.m_vertices.push_back(1.0f);
            }

            ///////////////////////////////////////
            // Add Buttom Right (Indice Node 1)
            ///////////////////////////////////////

            // View Space Coordiantes (i.e Normal Device Coordinates with respect to our orthogonal view)
            obj.m_openGlBuffer.m_vertices.push_back(xUpperBound); // X Pos
            obj.m_openGlBuffer.m_vertices.push_back(yLowerBound); // Y Pos
            obj.m_openGlBuffer.m_vertices.push_back(0.0f);        // Z Pos

            if (obj.m_textureUsed)
            {
                // Texture Coordinates
                obj.m_openGlBuffer.m_vertices.push_back(1.0f);
                obj.m_openGlBuffer.m_vertices.push_back(0.0f);
            }

            ///////////////////////////////////////
            // Add Buttom Left (Indice Node 2)
            ///////////////////////////////////////
                
            // View Space Coordiantes (i.e Normal Device Coordinates with respect to our orthogonal view)
            obj.m_openGlBuffer.m_vertices.push_back(xLowerBound); // X Pos
            obj.m_openGlBuffer.m_vertices.push_back(yLowerBound); // Y Pos
            obj.m_openGlBuffer.m_vertices.push_back(0.0f);        // Z Pos

            if (obj.m_textureUsed)
            {
                // Texture Coordinates
                obj.m_openGlBuffer.m_vertices.push_back(0.0f);
                obj.m_openGlBuffer.m_vertices.push_back(0.0f);
            }

            ///////////////////////////////////////
            // Add Top Left (Indice Node 3)
            ///////////////////////////////////////

            // View Space Coordiantes (i.e Normal Device Coordinates with respect to our orthogonal view)
            obj.m_openGlBuffer.m_vertices.push_back(xLowerBound); // X Pos
            obj.m_openGlBuffer.m_vertices.push_back(yUpperBound);  // Y Pos
            obj.m_openGlBuffer.m_vertices.push_back(0.0f);  // Z Pos

            if (obj.m_textureUsed)
            {
                // Texture Coordinates
                obj.m_openGlBuffer.m_vertices.push_back(0.0f);
                obj.m_openGlBuffer.m_vertices.push_back(1.0f);
            }

            // Define the unique indices used for drawing this object
            obj.m_openGlBuffer.m_indices.push_back(0);
            obj.m_openGlBuffer.m_indices.push_back(1);
            obj.m_openGlBuffer.m_indices.push_back(3);
            obj.m_openGlBuffer.m_indices.push_back(1);
            obj.m_openGlBuffer.m_indices.push_back(2);
            obj.m_openGlBuffer.m_indices.push_back(3);

            // Explictely define constants for readability
            obj.m_openGlBuffer.m_numUniqueIndices = 6;
            obj.m_openGlBuffer.m_numVertices = 4;
            obj.m_openGlBuffer.m_numDimensions = vertexPositionDimensions; // Assume that we're only pushing our Vertex Buffer with the X, Y, and Z values
            obj.m_openGlBuffer.m_numAttributeBuffers = 1; // Once again, assume only using Vertex Buffer X,Y, and Z position values for this buffer

            if (obj.m_textureUsed)
            {
                obj.m_openGlBuffer.m_numDimensions += texturePositionDimensions;
                obj.m_openGlBuffer.m_numAttributeBuffers++;
            }

            break;
        }
        default:
        {
            std::cout << label << "::" << __FUNCTION__ << ": Failed conversion of " << static_cast<int>(obj.m_type);
            conversionWasSuccess = false;
            break;
        }
    }   

    return conversionWasSuccess;
}

void Diegofied::populateOpenGlBuffers(Types::RenderObject& obj)
{
    /*
        IMPORTANT NOTE: For the sake of simplicity (although wildily inefficient in production),
        we are going to tie an instance of the VAO (Vertex Array Object), EBO (Element Buffer Object),
        and VBO (Vertex Buffer Object) to each individual object. This is ineffiencet since each call
        to tell the GPU to 1) Allocate memory and 2) Bind our object's VAO are expensive calls.

        Object Creation Step 3: 
    */
    
    // (1) Generate Buffers for our Vertex Array Object, Vertex Buffer Object, Element Buffer Object
    glGenVertexArrays(1, &obj.m_openGlBuffer.m_vertexArray);
    glBindVertexArray(obj.m_openGlBuffer.m_vertexArray);

    // (3) Then Bind and Buffer data into the Vertex Buffer Object
    glGenBuffers(1, &obj.m_openGlBuffer.m_vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, obj.m_openGlBuffer.m_vertexBuffer);
    float verticeBuffer[obj.m_openGlBuffer.m_vertices.size()];
    std::copy(obj.m_openGlBuffer.m_vertices.begin(), obj.m_openGlBuffer.m_vertices.end(), verticeBuffer);

    if (obj.m_openGlBuffer.m_renderType == Renderer::Types::RenderingType::STATIC)
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticeBuffer), verticeBuffer, GL_STATIC_DRAW);
    }
    else
    {
        glBufferData(GL_ARRAY_BUFFER, sizeof(verticeBuffer), verticeBuffer, GL_DYNAMIC_DRAW);
    }

    // (4) Then Bind and Buffer data into the Element Buffer Object
    glGenBuffers(1, &obj.m_openGlBuffer.m_elementBuffer);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, obj.m_openGlBuffer.m_elementBuffer);
    int indiceBuffer[obj.m_openGlBuffer.m_indices.size()];
    std::copy(obj.m_openGlBuffer.m_indices.begin(), obj.m_openGlBuffer.m_indices.end(), indiceBuffer);

    if (obj.m_openGlBuffer.m_renderType == Renderer::Types::RenderingType::STATIC)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indiceBuffer), indiceBuffer, GL_STATIC_DRAW);
    }
    else
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indiceBuffer), indiceBuffer, GL_DYNAMIC_DRAW);
    }

    // position attribute
    const unsigned int totalVertexBufferStride = obj.m_openGlBuffer.m_numDimensions * sizeof(obj.m_openGlBuffer.m_vertices[0]);
    unsigned int vertexBufferIndex = 0;
    const unsigned int byteOffset = vertexPositionDimensions;

    // For now, we always assume that whether we're using the color fields or not, whenever
    // textures are going to be used, that their buffer will exist inside the vertex array bufer
    if (obj.m_textureUsed)
    {   
        // Tell OpenGL how to traverse our Vertex Attributes (Position & Texture Only)

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, totalVertexBufferStride, (void*)0);
        glEnableVertexAttribArray(0);
          
        // Texture
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, totalVertexBufferStride, (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);
    }
    else
    {
        // Tell OpenGL how to traverse our Vertex Attributes (Position Only)

        // Position
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);  
    }

    // (6) For simplcity, always unbind our arrays, although depending on how performance critical we wish to be, 
    // we might want to change this. Being an assignment and all, let's opt for simplcity 
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // No need to unbind EBO, as this is handled by unbinding our VAO
    glBindVertexArray(0); 
}

////////////////////////////////////////////////////////////////////////////////
Shader::SubComponent::Diegofied* Diegofied::convertBaseShader(Shader::SubComponent::Base* base)
{
    return reinterpret_cast<Shader::SubComponent::Diegofied*>(base);
}
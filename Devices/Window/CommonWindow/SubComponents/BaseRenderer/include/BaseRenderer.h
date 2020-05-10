#pragma once
#include "BaseShader.h"

// Abstract interface for what our CommonRenderer
// class expects our Window Manager to provde to it
namespace Window
{
    namespace Device
    {
        class Manager
        {
            public:
                virtual ~Manager() = default;
                virtual void handleRefreshingWindow() = 0;
                virtual bool shouldRendererStopDrawing() = 0;
                virtual int getWindowWidth() = 0;
                virtual int getWindowHeight() = 0;
        };
    }
}

namespace Renderer
{
    namespace Types
    {
        // As noted in BaseShader, I'm just trying to provide us with some means in the future to not be too
        // tied to a specific library via typedefs.
        typedef Shader::Types::Vector3f Vector3f;

        // Colors we currently support
        namespace Colors
        {
            static Renderer::Types::Vector3f RED    = Renderer::Types::Vector3f(255.0f, 0.0f, 0.0f);
            static Renderer::Types::Vector3f BLUE   = Renderer::Types::Vector3f(0.0f, 0.0f, 255.0f);
            static Renderer::Types::Vector3f GREEN  = Renderer::Types::Vector3f(0.0f, 255.0f, 0.0f);
            static Renderer::Types::Vector3f YELLOW = Renderer::Types::Vector3f(255.0f, 255.0f, 0.0f);
            static Renderer::Types::Vector3f WHITE  = Renderer::Types::Vector3f(255.0f, 255.0f, 255.0f);
        };

        // Defines the set of objects we support View Space drawing of (i.e don't exist in Local or World Space (not actual objects))
        enum ViewSpaceObjects
        {
            TEXT
        };

        // Defines the set of objects that we support drawing of into World Space
        enum GeometricObjects
        {
            SQUARE
        };

        // Flag that identifies how often our Graphics Pipeline ought to expect these objects to change
        enum RenderingType
        {
            STATIC,
            DYNAMIC
        };
        
        // Flag that simply identifies whether the 3D object is going to use a texture or not
        enum Textures
        {
            NO_TEXTURE,
            USING_TEXTURE
        };

        /*
            Application Specific Logic - This struct defines essentially the 'children' metadata of our 3D objects (currently works for DYNAMIC only)
            TODO: If I had more time, abstract this out
        */
        struct MlbInfo
        {
            MlbInfo(std::string away, std::string home,
                    std::string inSeries, std::string gameNumber)
            : m_awayTeam(away),
              m_homeTeam(home),
              m_gamesInSeries(inSeries),
              m_seriesGameNumber(gameNumber)
            {
            }

            std::string m_awayTeam;
            std::string m_homeTeam;
            std::string m_gamesInSeries;
            std::string m_seriesGameNumber;
        };

    }

    namespace SubComponent
    {
        class Base
        {
            public:

                // Constructor
                Base(Window::Device::Manager& manager);
                
                // Destructor
                virtual ~Base() = default;

                // This interface is a blocking call that draws a variety of objects based upon either
                // statically created or dynamic objects (See RenderingType)
                virtual void drawToScreen(bool exitCondition) = 0;
                
                // This interface attempts to load a texture given a path and a handle
                virtual bool loadTexture(const char* texturePath, unsigned int& textureHandle) = 0;
                
                // This interface initalizes this sub component
                virtual bool initSubComponent() = 0;

                // This interface handles creation of view space objects only
                virtual bool createViewSpaceObject(const char* text,
                                                   Renderer::Types::Vector3f color,
                                                   Renderer::Types::Vector3f position,
                                                   Renderer::Types::Vector3f scale,
                                                   const char* vertexShaderPath,
                                                   const char* fragmentShaderPath) = 0;
                
                // This interface handles creation of world space (3D) objects
                virtual bool createGeometricObject(Renderer::Types::MlbInfo mlbInfo,
                                                   Renderer::Types::GeometricObjects type,
                                                   Renderer::Types::RenderingType renderType,
                                                   Renderer::Types::Vector3f color,
                                                   Renderer::Types::Textures textureType,
                                                   const char* texturePath,
                                                   Renderer::Types::Vector3f position,
                                                   Renderer::Types::Vector3f scale,
                                                   const char* vertexShaderPath,
                                                   const char* fragmentShaderPath) = 0;
            protected:
                // This interface determines whether this specific implementation supports creation
                // of 2D/3D objects
                virtual bool isGeometricObjectSupported(Renderer::Types::GeometricObjects type) = 0;

                // This interface determines whether this specific implementation supports handling
                // of the specific shader 
                virtual bool isShaderSupported(const char* path) = 0;

                // This interface is used to determine with the texture specified at the path is valid and supported
                virtual bool isTextureSupported(const char* path) = 0;

                // This is an interface that deals with communicating to the window manager
                Window::Device::Manager& m_windowManager;
        };
    }
}
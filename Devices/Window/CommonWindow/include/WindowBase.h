#pragma once

// Includes
#include <cstdint>

#include "BaseRenderer.h"

// Forward declarations
class GLFWmonitor;
class GLFWwindow;

namespace Window
{
    namespace Types
    {
        /* 
            Important Notes
            -----------------
            - In an ideal world, we'd have a class below that maps to whatever
              our Graphics API (OpenGl, Vulkin, etc) deems as a 'monitor' and 
              extend this functionality to our own Window Devices. Seeing as
              I have a very close coupling with my Graphics API (OPENGL), I'll typedef
              these away for now, but in production, we ought to have some 
              macros that resolve these 'types' to their respective Graphics API
        */

        typedef GLFWmonitor MonitorObject;
        typedef GLFWwindow WindowObject;

        // This could be handled more gracefully, say actually have some intelligence
        // about what platform I'm porting too and whether this is deemed 'acceptable' memory wise
        typedef uint16_t WindowDimensionsType;
        typedef uint8_t UuidType;
    }

    namespace Device
    {
        class Base : public Window::Device::Manager
        {
            public:
                Base(Window::Types::WindowDimensionsType width, 
                     Window::Types::WindowDimensionsType height,
                     const char* name, 
                     Window::Types::MonitorObject* monitor = nullptr);
                virtual ~Base();
                
                // With more time, I probably create a common device of some sorts, not just
                // what a common device is with respect to windows..
                virtual bool PrimeDevice() = 0;
                virtual void StartDevice() = 0;
                virtual void StopDevice() = 0;
                virtual void HandleWindowResize(Window::Types::WindowObject* window, int width, int height) = 0;

                // From Window::Device::Manager
                virtual void handleRefreshingWindow();
                virtual bool shouldRendererStopDrawing();
                virtual int getWindowWidth();
                virtual int getWindowHeight();

                // This is done intentionally to save time. In production, you'd stub out
                // any other functions, but seeing as I won't be using all of them, will
                // leave raw pointer accessible publically so I can get this assignment finish
                // and not over design this given it's scope.
                Window::Types::WindowObject* m_windowObj;
                Window::Types::WindowDimensionsType m_winWidth;
                Window::Types::WindowDimensionsType m_winHeight;
            protected:    
                bool m_deviceRunning;
                const char* m_winName;
                Window::Types::MonitorObject* m_monitorObj;

            private:
                virtual bool ExitPolicyNotSatisfied() = 0;
        };
    }
}
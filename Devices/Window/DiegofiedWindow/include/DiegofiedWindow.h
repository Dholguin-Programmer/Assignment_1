#pragma once

// C++ standard libraries
#include <memory>

// My libraries
#include "WindowBase.h"
#include "DiegofiedRenderer.h"

namespace Window
{
    namespace Device
    {
        class Diegofied : public Window::Device::Base
        {
            public:
                Diegofied();
                ~Diegofied() = default;

                // From Window::Device::Base
                bool PrimeDevice() override;
                void StartDevice() override;
                void StopDevice()  override;
                void HandleWindowResize(Window::Types::WindowObject* window, int width, int height) override;            
                void handleRefreshingWindow() override;
                bool shouldRendererStopDrawing() override;
                int getWindowWidth() override;
                int getWindowHeight() override;
                std::unique_ptr<Renderer::SubComponent::Diegofied> p_renderer;
                void handleKeyInput(int key, int scanecode, int action, int mods);

            private:
                // From Window::Device::Base
                bool ExitPolicyNotSatisfied() override;

                unsigned int m_currentObjectSelected;
        };
    }
}
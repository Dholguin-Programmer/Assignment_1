#include "WindowBase.h"
#include <iostream>

using namespace Window::Device;

////////////////////////////////////////////////////////////////////////////////
Base::Base(Window::Types::WindowDimensionsType width, 
           Window::Types::WindowDimensionsType height,
           const char* name, 
           Window::Types::MonitorObject* monitor)
: m_deviceRunning(false),
  m_winWidth(width),
  m_winHeight(height),
  m_winName(name),
  m_monitorObj(monitor),
  m_windowObj(nullptr)
{
    // Nothing common for now.. 
}

////////////////////////////////////////////////////////////////////////////////
Base::~Base()
{
    // Maybe we might want to come some common behavior here?
    // For now I'm treating this class more like a light-weight
    // struct that is enforcing behavior via inheritance
}

////////////////////////////////////////////////////////////////////////////////
bool Base::shouldRendererStopDrawing()
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;

    return false;
}

////////////////////////////////////////////////////////////////////////////////
void Base::handleRefreshingWindow()
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;
}

////////////////////////////////////////////////////////////////////////////////
int Base::getWindowWidth()
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;

    return 0;
}


////////////////////////////////////////////////////////////////////////////////
int Base::getWindowHeight()
{
    std::cout << "Override this functionality : " << __PRETTY_FUNCTION__ << std::endl;

    return 0;
}

#pragma once
#include "Window.h"
#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "OpenGL/Renderer.h"
#include "Timestep.h"
#include "Game/ParticleManager.h"
#include "ApplicationModels.h"
#include <array>
#include <cstdlib>

#define APP_BIND_EVENT(eventName) \
    m_window->m_data.callbackOn##eventName = std::bind(&Application::on##eventName, this, std::placeholders::_1)


/**
 * Base class for whole application.
 */
class Application
{
public:
    Application(int argc, char* argv[]);
    ~Application();

    /** 
     * Application's "main" function; contains game loop.
     */
    void run();

private:
    bool parseOptions(int argc, char* argv[]);

    void onWindowClose(WindowCloseEvent& e);
    void onWindowResize(WindowResizeEvent& e);
    void onKeyPress(KeyPressEvent& e);
    void onKeyRelease(KeyReleaseEvent& e);
    void onMouseMove(MouseMoveEvent& e);
    void onMouseButtonPress(MouseButtonPressEvent& e);
    void onMouseButtonRelease(MouseButtonReleaseEvent& e);

    bool m_isRunning;
    std::unique_ptr<Window> m_window;
    float m_lastFrameTime = 0.0f;
    float m_mousePosX = 0.0f;
    float m_mousePosY = 0.0f;

    AppConfig m_config;
    ParticleManager m_manager;
};


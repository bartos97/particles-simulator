#pragma once
#include "Window.h"
#include "Events/Event.h"
#include "Events/WindowEvents.h"
#include "Events/KeyEvents.h"
#include "Events/MouseEvents.h"
#include "OpenGL/Renderer.h"
#include "Timestep.h"
#include "Game/ParticleManager.h"

#define APP_BIND_EVENT(eventName) \
    m_window->m_data.callbackOn##eventName = std::bind(&Application::on##eventName, this, std::placeholders::_1)

/**
 * Singleton. Base class for whole application.
 */
class Application
{
private:
    Application();

public:
    ~Application();

    /**
     * Object initialization function
     * @return Pointer to (only) instance of this class
     */
    static Application* getInstance();

    /** 
     * Application's "main" function; contains game loop.
     */
    void run();

private:
    void onWindowClose(WindowCloseEvent& e);
    void onWindowResize(WindowResizeEvent& e);
    void onKeyPress(KeyPressEvent& e);
    void onKeyRelease(KeyReleaseEvent& e);
    void onMouseMove(MouseMoveEvent& e);
    void onMouseButtonPress(MouseButtonPressEvent& e);
    void onMouseButtonRelease(MouseButtonReleaseEvent& e);

    bool m_isRunning;
    static Application* m_instance;
    std::unique_ptr<Window> m_window;
    float m_lastFrameTime = 0.0f;
};


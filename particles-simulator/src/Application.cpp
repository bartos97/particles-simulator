#include "pch.h"
#include "Application.h"
#include "Game/ParticleRenderer.h"
#include <random>
#include <cmath>

Application::Application(int argc, char* argv[])
{
    PS_LOG("Application constructed.");

    bool optionsValid = parseOptions(argc, argv);
    if (!optionsValid)
    {
        PS_INFO("No options passed or none is valid; using default ones");
    }

    m_window = std::unique_ptr<Window>(Window::getInstance());
    m_isRunning = true;
    m_manager = ParticleManager(m_config);

    APP_BIND_EVENT(WindowClose);
    APP_BIND_EVENT(WindowResize);
    APP_BIND_EVENT(KeyPress);
    APP_BIND_EVENT(KeyRelease);
    APP_BIND_EVENT(MouseMove);
    APP_BIND_EVENT(MouseButtonPress);
    APP_BIND_EVENT(MouseButtonRelease);
}

Application::~Application()
{
    PS_LOG("Application destroyed.");
}

void Application::run()
{
    PS_LOG("App starts running.");

    PS_LOG("Entering the game loop");
    while (m_isRunning)
    {
        Renderer::clearScreen();

        float time = float(glfwGetTime());
        Timestep timestep = time - m_lastFrameTime;
        m_lastFrameTime = time;

        m_manager.onUpdate(timestep);

        m_window->onUpdate();
    }
    PS_LOG("Returned from game loop");
}


bool Application::parseOptions(int argc, char* argv[])
{
    if (argc <= 2) return false;

    static const AppExecParam num = {
        "--num",
        [&](const char* param)->bool {
            size_t num = std::atoll(param);
            if (num > 0)
            {
                m_config.num = num;
                m_config.numSet = true;
                return true;
            }
            return false;
        }
    };

    static const AppExecParam path = {
        "--path",
        [&](const char* param)->bool {
            m_config.path = Core::PROJECT_ABS_PATH + "../" + std::string(param);
            m_config.pathSet = true;
            return true;
        }
    };

    static const AppExecParam speedFrom = {
        "--speedFrom",
        [&](const char* param)->bool {
            float num = std::atof(param);
            if (num > 0.0f)
            {
                m_config.speedFrom = num;
                return true;
            }
            return false;
        }
    };

    static const AppExecParam speedTo = {
        "--speedTo",
        [&](const char* param)->bool {
            float num = std::atof(param);
            if (num > 0.0f)
            {
                m_config.speedTo = num;
                return true;
            }
            return false;
        }
    };

    static const AppExecParam radiusFrom = {
        "--radiusFrom",
        [&](const char* param)->bool {
            float num = std::atof(param);
            if (num > 0.0f)
            {
                m_config.radiusFrom = num;
                return true;
            }
            return false;
        }
    };

    static const AppExecParam radiusTo = {
        "--radiusTo",
        [&](const char* param)->bool {
            float num = std::atof(param);
            if (num > 0.0f)
            {
                m_config.radiusTo = num;
                return true;
            }
            return false;
        }
    };

    static const std::array<const AppExecParam, 6> options = { num, path, speedFrom, speedTo, radiusFrom, radiusTo };
    
    bool anyValidOption = false;
    for (int i = 1; i < argc; i++)
    {
        for (auto& option : options)
        {
            if (std::strcmp(argv[i], option.key) == 0 && i < argc - 1)
            {
                anyValidOption = option.callback(argv[i + 1]);
                i++;
                break;
            }
        }
    }

    return anyValidOption;
}

void Application::onWindowClose(WindowCloseEvent & e)
{
    PS_EVENT_LOG(e, "Window closed.");
    m_isRunning = false;
    e.m_isHandled = true;
}


void Application::onWindowResize(WindowResizeEvent & e)
{
    PS_EVENT_LOG(e, "Window resized to %d x %d", e.getWidth(), e.getHeight());
    e.m_isHandled = true;
}


void Application::onKeyPress(KeyPressEvent & e)
{
    int code = e.getKeyCode();
    if (code < 128)
    {
        PS_EVENT_LOG(e, "Key %c (#%d) pressed, repeated % d", code, code, e.getRepeatState());
    }
    else
    {
        PS_EVENT_LOG(e, "Key #%d pressed, repeated % d", code, e.getRepeatState());
    } 
    
    switch (code)
    {
        case GLFW_KEY_ESCAPE:
        {
            m_isRunning = false;
            break;
        }
        case GLFW_KEY_SPACE:
        {
            m_manager.toggleSimulation();
            break;
        }
        case GLFW_KEY_S:
        {
            m_manager.saveState();
            break;
        }
        case GLFW_KEY_L:
        {
            m_manager.loadState();
            break;
        }
    }

    e.m_isHandled = true;
}


void Application::onKeyRelease(KeyReleaseEvent & e)
{
    int code = e.getKeyCode();
    if (code < 128)
    {
        PS_EVENT_LOG(e, "Key %c (#%d) released", code, code);
    }
    else
    {
        PS_EVENT_LOG(e, "Key #%d released", code);
    } 
    e.m_isHandled = true;
}


void Application::onMouseMove(MouseMoveEvent & e)
{
    //PS_EVENT_LOG(e, "Mouse move at x:%lf y:%lf", e.getX(), e.getY());

    // mouse position received from event are in coordinates system
    // where origin is at window's top left corner, so
    // position (in px) with respect to center of window wiil be:
    m_mousePosY = float(m_window->m_data.windowCenterY - e.getY());
    m_mousePosX = float(e.getX() - m_window->m_data.windowCenterX);

    //x and y in normalized device coordinates, i.e. [-1, 1]
    m_mousePosX = float(m_mousePosX / m_window->m_data.windowCenterX);
    m_mousePosY = float(m_mousePosY / m_window->m_data.windowCenterY);

    m_manager.onMouseMove(m_mousePosX, m_mousePosY);

    e.m_isHandled = true;
}


void Application::onMouseButtonPress(MouseButtonPressEvent & e)
{
    PS_EVENT_LOG(e, "Mouse button #%d pressed", e.getButton());
    if (e.getButton() == 0)
    {
        m_manager.onMousePress();
    }
    e.m_isHandled = true;
}


void Application::onMouseButtonRelease(MouseButtonReleaseEvent & e)
{
    PS_EVENT_LOG(e, "Mouse button #%d released", e.getButton());
    if (e.getButton() == 0)
    {
        m_manager.onMouseRelease();
    }
    e.m_isHandled = true;
}
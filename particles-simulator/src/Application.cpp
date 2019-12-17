#include "pch.h"
#include "Application.h"
#include "Game/ParticleRenderer.h"
#include <random>
#include <cmath>

// Declaration of static variable
Application* Application::m_instance;

Application::Application()
{
    PS_LOG("Application constructed.");
    m_window = std::unique_ptr<Window>(Window::getInstance());

    APP_BIND_EVENT(WindowClose);
    APP_BIND_EVENT(WindowResize);
    APP_BIND_EVENT(KeyPress);
    APP_BIND_EVENT(KeyRelease);
    APP_BIND_EVENT(MouseMove);
    APP_BIND_EVENT(MouseButtonPress);
    APP_BIND_EVENT(MouseButtonRelease);

    //Renderer::setMode(GL_LINE);
#ifdef PS_DEBUG
#else
#endif // PS_DEBUG
    Renderer::setMode(GL_FILL);

    m_isRunning = true;
}

Application::~Application()
{
    PS_LOG("Application destroyed.");
}


Application * Application::getInstance()
{
    if (!m_instance) m_instance = new Application();
    return m_instance;
}


void Application::run()
{
    PS_LOG("App starts running.");

    const float vertices[] = {
        -1.0f, 1.0f,
        1.0f, 1.0f,
        1.0f, -1.0f,
        -1.0f, -1.0f
    };
    const unsigned int indecies[] = {
        0, 1, 2,
        2, 3, 0
    };

    std::string vertexShaderPath = Core::PROJECT_ABS_PATH + "res/Shaders/circle.vert.glsl";
    std::string fragmentShaderPath = Core::PROJECT_ABS_PATH + "res/Shaders/circle.frag.glsl";
    auto circleShader = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    BufferLayout layoutVertices2D;
    layoutVertices2D.add<float>(2);

    auto va = std::make_shared<VertexArray>();
    VertexBuffer vb = VertexBuffer(sizeof(float) * 8, vertices);
    ElementBuffer eb = ElementBuffer(6, indecies);
    va->assignData(vb, eb, layoutVertices2D);

    ParticleRenderer renderer = ParticleRenderer(circleShader, va);
    Particle asd = Particle();

    /*Particle particles[500];
    for (size_t i = 0; i < 500; i++)
    {
        particles[i] = Particle(
            glm::vec2(float(pow(-1, rand()) * rand()) / RAND_MAX, float(pow(-1, rand()) * rand()) / RAND_MAX),
            glm::vec2(0.0f, 0.0f),
            0.025f
        );
    }*/

    PS_LOG("Entering the game loop");
    while (m_isRunning)
    {
        Renderer::clearScreen();

        float sin = std::sin(glfwGetTime());
        asd.setPosition(glm::vec2(sin, sin));
        renderer.render(asd);

        /*for (size_t i = 0; i < 500; i++)
        {
            particles[i].setPosition(glm::vec2(float(pow(-1, rand()) * rand()) / RAND_MAX, float(pow(-1, rand()) * rand()) / RAND_MAX));
            renderer.render(particles[i]);
        }*/

        m_window->onUpdate();
    }
    PS_LOG("Returned from game loop");
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
        //case GLFW_KEY_UP:
        //{
        //    break;
        //}
        //case GLFW_KEY_DOWN:
        //{
        //    break;
        //}
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
    PS_EVENT_LOG(e, "Mouse move at x:%lf y:%lf", e.getX(), e.getY());

    // mouse position received from event are in coordinates system
    // where origin is at window's top left corner, so
    // position (in px) with respect to center of window wiil be:
    double mouseY = m_window->m_data.windowCenterY - e.getY();
    double mouseX = e.getX() - m_window->m_data.windowCenterX;

    //x and y in normalized device coordinates, i.e. [-1, 1]
    mouseX = mouseX / m_window->m_data.windowCenterX;
    mouseY = mouseY / m_window->m_data.windowCenterY;

    this->mousePosition.x = float(mouseX);
    this->mousePosition.y = float(mouseY);

    e.m_isHandled = true;
}


void Application::onMouseButtonPress(MouseButtonPressEvent & e)
{
    PS_EVENT_LOG(e, "Mouse button #%d pressed", e.getButton());
    e.m_isHandled = true;
}


void Application::onMouseButtonRelease(MouseButtonReleaseEvent & e)
{
    PS_EVENT_LOG(e, "Mouse button #%d released", e.getButton());
    e.m_isHandled = true;
}
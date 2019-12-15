#include "pch.h"
#include "Application.h"


// Declaration of static variable
Application* Application::m_instance;


Application::Application()
{
    PS_LOG("Application constructed.");
    m_window = std::unique_ptr<Window>(Window::create());

    APP_BIND_EVENT(WindowClose);
    APP_BIND_EVENT(WindowResize);
    APP_BIND_EVENT(KeyPress);
    APP_BIND_EVENT(KeyRelease);
    APP_BIND_EVENT(MouseMove);
    APP_BIND_EVENT(MouseButtonPress);
    APP_BIND_EVENT(MouseButtonRelease);

#ifdef PS_DEBUG
    Renderer::setMode(GL_LINE);
#else
    Renderer::setMode(GL_FILL);
#endif // PS_DEBUG

    m_isRunning = true;
}


Application::~Application()
{
    PS_LOG("Application destroyed.");
}


Application * Application::create()
{
    if (!m_instance) m_instance = new Application();
    return m_instance;
}


void Application::run()
{
    PS_LOG("App starts running.");

    PlayerModel::generateModel();
    BallModel::generateModel();

    std::string vertexShaderPath = Core::PROJECT_ABS_PATH + "res/Shaders/basic.vert";
    std::string fragmentShaderPath = Core::PROJECT_ABS_PATH + "res/Shaders/basic.frag";

    auto basicShaderPtr = std::make_shared<Shader>(vertexShaderPath.c_str(), fragmentShaderPath.c_str());

    BufferLayout layoutVertices2D;
    layoutVertices2D.add<float>(2);

    auto playerVertexArrayPtr = std::make_shared<VertexArray>();
    VertexBuffer playerVertexBuffer(unsigned int(PlayerModel::getVertices().size() * sizeof(float)), 
                                    PlayerModel::getVertices().data());
    ElementBuffer playerElementBuffer(unsigned int(PlayerModel::getIndecies().size()), 
                                      PlayerModel::getIndecies().data());
    playerVertexArrayPtr->assignData(playerVertexBuffer, playerElementBuffer, layoutVertices2D);

    auto ballVertexArrayPtr = std::make_shared<VertexArray>();
    VertexBuffer ballVertexBuffer(unsigned int(BallModel::getVertices().size() * sizeof(float)), 
                                  BallModel::getVertices().data());
    ElementBuffer ballElementBuffer(unsigned int(BallModel::getIndecies().size()), 
                                    BallModel::getIndecies().data());
    ballVertexArrayPtr->assignData(ballVertexBuffer, ballElementBuffer, layoutVertices2D);

    m_userPlayer = std::make_unique<Player>(basicShaderPtr, playerVertexArrayPtr, 0.0);
    m_opponentPlayer = std::make_unique<Player>(basicShaderPtr, playerVertexArrayPtr, M_PI);
    m_gameBall = std::make_unique<Ball>(basicShaderPtr, ballVertexArrayPtr, glm::vec2(0.0f, 0.0f));

    PS_LOG("Entering the game loop");
    while (m_isRunning)
    {
        Renderer::clearScreen();

        checkForCollisions();
        m_userPlayer->render();
        m_opponentPlayer->render();
        m_gameBall->render();

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
    PS_EVENT_LOG(e, "Key #%d pressed, repeated %d", code, e.getRepeatState());
    
    switch (code)
    {
    case GLFW_KEY_ESCAPE:
    {
        m_isRunning = false;
        break;
    }
    case GLFW_KEY_UP:
    {
        m_userPlayer->goUp();
        break;
    }
    case GLFW_KEY_DOWN:
    {
        m_userPlayer->goDown();
        break;
    }
    }

    e.m_isHandled = true;
}


void Application::onKeyRelease(KeyReleaseEvent & e)
{
    PS_EVENT_LOG(e, "Key #%d released", e.getKeyCode());
    e.m_isHandled = true;
}


void Application::onMouseMove(MouseMoveEvent & e)
{
    // mouse position received from event are in coordinates system
    // where origin is at window's top left corner, so
    // position (in px) with respect to center of window wiil be:
    double mouseY = m_window->m_data.windowCenterY - e.getY();
    double mouseX = e.getX() - m_window->m_data.windowCenterX;   

    double angle = std::atan2(mouseY, mouseX);
    
#ifdef PS_DEBUG
    //x and y in normalized device coordinates, i.e. [-1, 1]
    mouseX = mouseX / m_window->m_data.windowCenterX;
    mouseY = mouseY / m_window->m_data.windowCenterY;
    m_gameBall->moveTo(glm::vec2(mouseX, mouseY));
#endif // PS_DEBUG

    PS_EVENT_LOG(e, "Mouse move at x:%lf y:%lf", mouseX, mouseY);

    if (mouseX > 0.0 && angle < PlayerModel::maxPositionAngle && angle > PlayerModel::minPositionAngle)
    {
        m_userPlayer->setPosition(angle);
    }

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


void Application::checkForCollisions()
{
    if (m_gameBall->checkBounds())
    {
        if (m_userPlayer->checkCollision(m_gameBall))
        {
            PS_LOG("Ball collides with user's Player");
            m_gameBall->bounce(true);
        }
        else if (m_opponentPlayer->checkCollision(m_gameBall))
        {
            PS_LOG("Ball collides with automatic Player");
            m_gameBall->bounce(true);
        }
        else
        {
            if (m_gameBall->getPosition().x < 0.0)
            {
                PS_LOG("User wins");
                m_gameBall->bounce();
            }
            else
            {
                PS_LOG("User loses");
                m_gameBall->bounce();
            }

            //TODO: reset game
        }
    }
}
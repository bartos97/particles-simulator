#include "pch.h"
#include "Ball.h"
#include "PlayerModel.h"
#include "BallModel.h"
#include <random>


const glm::mat4 Ball::IDENTITY_MAT4 = glm::mat4(1.0f);
const float Ball::BASE_VELOCITY = 2.0f / 80.0f;


Ball::Ball(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> va, const glm::vec2& startPos)
{
    if (!BallModel::isCreated())
        BallModel::generateModel();

    m_modelShader = shader;
    m_modelVertexArray = va;
    m_velocity = Ball::BASE_VELOCITY;
    m_acceleration = -m_velocity / 150.0f;
    m_position = startPos;
    m_positionDisplacement = glm::vec2(0.0f, 0.0f);
    m_directionUnitVector = m_positionDisplacement;
    m_transformMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(m_position.x, m_position.y, 0.0f));
    moveTo(generateRandomVector());
    
    PS_LOG("Ball created at position: x = %f, y = %f", m_position.x ,m_position.y);
}


Ball::Ball(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> va)
    : Ball(shader, va, generateRandomVector())
{}


const glm::vec2& Ball::getPosition() const
{
    return m_position;
}


const glm::vec2& Ball::getDirection() const
{
    return m_directionUnitVector;
}


bool Ball::checkBounds()
{
    float distance = glm::distance(m_position + m_positionDisplacement, glm::vec2(0.0f, 0.0f));
    return double(distance) + BallModel::radius > PlayerModel::innerRadius;
}


void Ball::setDirection(const glm::vec2& direction)
{
    m_directionUnitVector = glm::normalize(direction);
}


void Ball::moveTo(const glm::vec2& newPosition)
{
    glm::vec2 moveVec(newPosition.x - m_position.x, newPosition.y - m_position.y);
    setDirection(moveVec);
}


void Ball::bounce(bool applyForce)
{
    glm::vec2 surfaceNormal(-m_position.x, -m_position.y);
    surfaceNormal = glm::normalize(surfaceNormal);
    glm::vec2 reflectionVector = glm::reflect(m_directionUnitVector, surfaceNormal);

    if (applyForce)
        m_velocity = Ball::BASE_VELOCITY;
    m_directionUnitVector = reflectionVector;
}


void Ball::render()
{
    nextStep();
    m_modelShader->setUniform("u_transformation", m_transformMatrix);
    Renderer::draw(m_modelVertexArray, m_modelShader);
}


void Ball::setPosition(const glm::vec2& newPosition)
{
    m_position = newPosition;
    m_transformMatrix = glm::translate(IDENTITY_MAT4, glm::vec3(m_position.x, m_position.y, 0.0f));
}


void Ball::nextStep()
{
    if (m_velocity > Ball::BASE_VELOCITY / 2.0f)
        m_velocity += m_acceleration;
    m_positionDisplacement = m_directionUnitVector * m_velocity;
    m_position += m_positionDisplacement;
    m_transformMatrix = glm::translate(IDENTITY_MAT4, glm::vec3(m_position.x, m_position.y, 0.0f));
}


glm::vec2 Ball::generateRandomVector()
{
    glm::vec2 returnVec;
    std::random_device rd;
    std::mt19937 generator(rd());
    std::uniform_real_distribution<float> intPosDistribution(0.0f, 1.0f);
    std::bernoulli_distribution signDistribution(0.5);

    float tmpX = intPosDistribution(generator);
    float tmpY = intPosDistribution(generator);
    if (!signDistribution(generator)) tmpX = -tmpX;
    if (!signDistribution(generator)) tmpY = -tmpY;
    returnVec = { tmpX, tmpY };

    float maxRadius = float(PlayerModel::innerRadius - BallModel::radius - 0.01);

    if (glm::distance(returnVec, glm::vec2(0.0f, 0.0f)) > maxRadius)
    {
        //angle between x axis and new random point
        double theta = std::atan2(returnVec.y, returnVec.x);

        //point colinear with new random point, but inside game circle
        glm::vec2 pointOnCircle(std::cos(theta) * maxRadius,
                                std::sin(theta) * maxRadius);
        returnVec = pointOnCircle;
    }

    return returnVec;
}


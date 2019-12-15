#include "pch.h"
#include "Player.h"


Player::Player(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> va, double startAngle)
{
    if (!PlayerModel::isCreated())
        PlayerModel::generateModel();

    PS_LOG("Player created");
    m_modelShader = shader;
    m_modelVertexArray = va;
    m_rotationAngle = 0.0;
    m_angleIncrement = float(PlayerModel::maxPositionAngle / 10.0);
    m_transformMatrix = PlayerModel::identityMat4;

    setPosition(startAngle);
}


float Player::getPositionAngle() const
{
    return m_rotationAngle;
}


void Player::setPosition(double angle)
{
    setPosition(float(angle));
}


void Player::setPosition(float angle)
{
    m_rotationAngle = angle;
    m_transformMatrix = glm::rotate(PlayerModel::identityMat4, m_rotationAngle, PlayerModel::rotationUnitVector);
}


void Player::goUp()
{
    float newAngle = m_rotationAngle + m_angleIncrement;
    if (newAngle < PlayerModel::maxPositionAngle)
    {
        setPosition(newAngle);
    }
    else
    {
        setPosition(PlayerModel::maxPositionAngle);
    }
}


void Player::goDown()
{
    float newAngle = m_rotationAngle - m_angleIncrement;
    if (newAngle > PlayerModel::minPositionAngle)
    {
        setPosition(newAngle);
    }
    else
    {
        setPosition(PlayerModel::minPositionAngle);
    }
}


bool Player::checkCollision(const std::unique_ptr<Ball>& ball) const
{
    return checkCollision(*(ball.get()));
}


bool Player::checkCollision(const Ball& ball) const
{
    double modelHalfAngleSize = PlayerModel::modelSizeAngle / 2.0;
    glm::vec2 ballPosition = ball.getPosition();
    glm::vec2 ballDirection = ball.getDirection();

    //vector perpendicular clockwise to ball's direction vector
    glm::vec2 ballPerpCW(ballDirection.y, -ballDirection.x);
    ballPerpCW = ballPerpCW * float(BallModel::radius);

    //vector perpendicular counter clockwise to ball's direction vector
    glm::vec2 ballPerpCCW(-ballDirection.y, ballDirection.x);
    ballPerpCCW = ballPerpCCW * float(BallModel::radius);

    double ballAngleCCW = std::atan2(ballPosition.y + ballPerpCCW.y, ballPosition.x + ballPerpCCW.x);
    double ballAngleCW = std::atan2(ballPosition.y + ballPerpCW.y, ballPosition.x + ballPerpCW.x);

    double angleDistanceCCW = std::abs(ballAngleCCW - double(m_rotationAngle));
    if (angleDistanceCCW > M_PI)
        angleDistanceCCW = 2 * M_PI - angleDistanceCCW;

    double angleDistanceCW = std::abs(ballAngleCW - double(m_rotationAngle));
    if (angleDistanceCW > M_PI)
        angleDistanceCW = 2 * M_PI - angleDistanceCW;

    double smallestDistance;
    angleDistanceCCW < angleDistanceCW ? smallestDistance = angleDistanceCCW : smallestDistance = angleDistanceCW;

    //PS_LOG("Collision log:\nBall angle CCW: %lf Ball angle CW: %lf\nPlayer angle: %lf\nDistance angle: %lf",
    //       ballAngleCCW, ballAngleCW, m_rotationAngle, smallestDistance);

    return smallestDistance <= modelHalfAngleSize;
}


void Player::render() const
{
    m_modelShader->setUniform("u_transformation", m_transformMatrix);
    Renderer::draw(m_modelVertexArray, m_modelShader);
}

#pragma once
#include "OpenGL/Renderer.h"
#include "PlayerModel.h"
#include "Ball.h"


class Player
{
public:
    Player(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> va, double startAngle);
    
    float getPositionAngle() const;

    void setPosition(double angle);
    void setPosition(float angle);
    void goUp();
    void goDown();

    bool checkCollision(const std::unique_ptr<Ball>& ball) const;
    bool checkCollision(const Ball& ball) const;

    void render() const;

private:    
    std::shared_ptr<Shader> m_modelShader;
    std::shared_ptr<VertexArray> m_modelVertexArray;

    float m_rotationAngle;
    float m_angleIncrement;
    glm::mat4 m_transformMatrix;
};


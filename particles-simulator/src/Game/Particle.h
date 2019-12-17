#pragma once

class Particle
{
public:
    Particle();
    Particle(const glm::vec2& position, const glm::vec2& speed, float radius = 0.05f);

    float getRadius() const { return m_radius; }
    const glm::vec2& getPosition() const { return m_position; }
    const glm::vec2& getSpeed() const { return m_speed; }

    void setPosition(const glm::vec2& position);
    void setSpeed(const glm::vec2& speed);
    void setRadius(float radius);

private:

private:
    glm::vec2 m_position;
    glm::vec2 m_speed;
    float m_radius;
};

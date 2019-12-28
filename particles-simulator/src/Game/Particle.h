#pragma once
#include "Timestep.h"
#include <sstream>
#include <limits>

class Particle
{
public:
    Particle(const glm::vec2& position = glm::vec2(0.0f, 0.0f),
             const glm::vec2& speed = glm::vec2(0.0f, 0.0f),
             float radius = 0.05f,
             const glm::vec4& color = glm::vec4(0.2f, 0.5f, 0.6f, 1.0f));

    bool operator==(const Particle& p) const
    {
        return p.getID() == m_ID;
    }

    unsigned int getID() const
    {
        return m_ID;
    }

    float getRadius() const
    {
        return m_radius;
    }

    const glm::vec2& getPosition() const
    {
        return m_position;
    }

    const glm::vec2& getSpeed() const
    {
        return m_speed;
    }

    const glm::vec4& getColor() const
    {
        return m_color;
    }

    glm::vec2 getPositionInNextFrame(Timestep ts) const
    {
        return m_position + m_speed * float(ts);
    }

    void setSpeed(const glm::vec2& speed)
    {
        m_speed = speed;
    }

    void setSpeed(float x, float y)
    {
        m_speed.x = x;
        m_speed.y = y;
    }

    void setColor(const glm::vec4& color)
    {
        m_color = color;
    }

    void setColor(float r, float g, float b, float alpha)
    {
        m_color.x = r;
        m_color.y = g;
        m_color.z = b;
        m_color.w = alpha;
    }

    void setRadius(float radius)
    {
        m_radius = radius;
    }

    void setPosition(const glm::vec2& position)
    {
        m_position = position;
    }

    void setPosition(float x, float y)
    {
        m_position.x = x;
        m_position.y = y;
    }

    void restoreColor()
    {
        m_color = m_defaultColor;
    }

    std::string getDescription() const;
    void setSpeedWithCollisionCheck(const glm::vec2& speed, std::vector<Particle>& particles, Particle& collisionOriginator, Timestep ts);
    void onUpdate(Timestep ts);
    void onCollision(Particle& collisionOriginator, std::vector<Particle>& allOtherParticles, Timestep ts);

private:
    void updateColor(Timestep ts);

public:
    static const float COLLISION_COLOR_ANIMATION_TIME;
    static const glm::vec4 COLLISION_COLOR;
    float m_area;

private:
    unsigned int m_ID;
    glm::vec2 m_position;
    glm::vec2 m_speed; //units per second
    float m_radius;
    glm::vec4 m_color;
    const glm::vec4 m_defaultColor;
    const glm::vec4 m_collisionColorComplement;
    float m_timeSinceCollision = std::numeric_limits<float>::max();
};

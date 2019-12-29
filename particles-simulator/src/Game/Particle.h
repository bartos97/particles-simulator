#pragma once
#include "Timestep.h"
#include <sstream>

class Particle
{
public:
    static const float COLLISION_COLOR_ANIMATION_TIME;
    static const glm::vec4 COLLISION_COLOR;

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

    float getArea() const
    {
        return m_area;
    }

    float getMass() const
    {
        return m_area;
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
        if (radius < 0.025f) return;
        m_radius = radius;
        m_area = float(M_PI * m_radius * m_radius);
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

    bool isInside(const glm::vec2& position)
    {
        return glm::distance(m_position, position) <= m_radius;
    }

    bool isInside(float x, float y)
    {
        float distance = sqrtf((x - m_position.x) * (x - m_position.x) + (y - m_position.y) * (y - m_position.y));
        return distance <= m_radius;
    }

    std::string getDescription() const;
    void onUpdate(Timestep ts, bool updatePosition);
    void onCollision();    

private:
    void updateColor(Timestep ts);

private:
    unsigned int m_ID;
    glm::vec2 m_position;
    glm::vec2 m_speed; //units per second
    float m_radius;
    float m_area;
    glm::vec4 m_color;
    const glm::vec4 m_defaultColor;
    const glm::vec4 m_collisionColorComplement;
    float m_timeSinceCollision = Particle::COLLISION_COLOR_ANIMATION_TIME + 1e-5f;
};

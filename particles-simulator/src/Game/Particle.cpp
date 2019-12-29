#include "pch.h"
#include "Particle.h"

const float Particle::COLLISION_COLOR_ANIMATION_TIME = 0.5f;
const glm::vec4 Particle::COLLISION_COLOR = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

Particle::Particle(const glm::vec2& position, const glm::vec2& speed, float radius, const glm::vec4& color)
    : m_position(position), m_speed(speed), m_radius(radius), m_area(float(M_PI * m_radius * m_radius)),
      m_color(color), m_defaultColor(color), m_collisionColorComplement(COLLISION_COLOR - color)
{
    static unsigned int counter;
    m_ID = counter;
    counter++;;
}

void Particle::onUpdate(Timestep ts, bool updatePosition)
{
    updateColor(ts);
    m_timeSinceCollision += ts;
    if (updatePosition)
    {
        m_position += m_speed * float(ts);
    }
}

void Particle::onCollision()
{
    m_timeSinceCollision = 0.0f;
}

void Particle::updateColor(Timestep ts)
{
    if (m_timeSinceCollision <= COLLISION_COLOR_ANIMATION_TIME)
    {
        auto colorDelta = m_collisionColorComplement * (float(ts) / COLLISION_COLOR_ANIMATION_TIME / 0.5f);
        if (m_timeSinceCollision <= COLLISION_COLOR_ANIMATION_TIME / 2.0f)
        {
            if (m_color.x < 1.0f) m_color += colorDelta;
        }
        else
        {
            m_color -= colorDelta;
        }
    }
    else
    {
        restoreColor();
    }
}

std::string Particle::getDescription() const
{
    std::ostringstream stream;
    stream << "Particle #"<< m_ID << " r:" << m_radius << " at x:" << m_position.x << " y:" << m_position.y;
    return stream.str();
}

#include "pch.h"
#include "Particle.h"

const float Particle::COLLISION_COLOR_ANIMATION_TIME = 0.5f;
const glm::vec4 Particle::COLLISION_COLOR = glm::vec4(0.8f, 0.0f, 0.0f, 1.0f);

Particle::Particle(const glm::vec2& position, const glm::vec2& speed, float radius, const glm::vec4& color)
    : m_position(position), m_speed(speed), m_radius(radius), m_color(color), m_defaultColor(color), m_collisionColorComplement(COLLISION_COLOR - color)
{
    static unsigned int counter;
    m_area = M_PI * m_radius * m_radius;
    m_ID = counter;
    counter++;
}

void Particle::setSpeedWithCollisionCheck(const glm::vec2& speed, std::vector<Particle>& particles, Particle& collisionOriginator, Timestep ts)
{
    m_timeSinceCollision = 0.0f;
    auto newPos = m_position + speed * float(ts);
    bool hasColided = false;
    for (auto& p : particles)
    {
        if (p.getID() == m_ID || collisionOriginator.getID() == p.getID()) continue;
        auto dist = glm::distance(newPos, p.getPositionInNextFrame(ts)) - 0.005f;
        float radiusSum = p.getRadius() + m_radius;
        if (dist <= radiusSum)
        {
            p.setSpeedWithCollisionCheck(speed, particles, *this, ts);
            setSpeed(0.0f, 0.0f);
            hasColided = true;
        }
    }
    if(!hasColided) setSpeed(speed);
}

void Particle::onUpdate(Timestep ts)
{
    updateColor(ts);
    m_position += m_speed * float(ts);
    m_timeSinceCollision += ts;
}

void Particle::onCollision(Particle& collisionOriginator, std::vector<Particle>& allOtherParticles, Timestep ts)
{
    m_timeSinceCollision = 0.0f;
    const glm::vec2 firstSpeed = glm::vec2(m_speed);
    const glm::vec2 secondSpeed = glm::vec2(collisionOriginator.getSpeed());
    setSpeedWithCollisionCheck(secondSpeed, allOtherParticles, collisionOriginator, ts);
    collisionOriginator.setSpeedWithCollisionCheck(firstSpeed, allOtherParticles, *this, ts);
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

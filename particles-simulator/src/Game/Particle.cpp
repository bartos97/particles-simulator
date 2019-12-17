#include "pch.h"
#include "Particle.h"

Particle::Particle()
    : Particle(glm::vec2(0.0f, 0.0f), glm::vec2(0.0f, 0.0f))
{
   
}

Particle::Particle(const glm::vec2& position, const glm::vec2& speed, float radius)
    : m_position(position), m_speed(speed), m_radius(radius)
{

}

void Particle::setPosition(const glm::vec2& position) 
{ 
    m_position = position;
}

void Particle::setSpeed(const glm::vec2& speed)
{
    m_speed = speed;
}

void Particle::setRadius(float radius)
{
    m_radius = radius;
}

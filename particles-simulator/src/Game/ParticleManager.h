#pragma once
#include "Particle.h"
#include "Timestep.h"
#include "ParticleRenderer.h"
#include <random>
#include <cmath>


class ParticleManager
{
public:
    using CollisionPair = std::pair<Particle&, Particle&>;

    ParticleManager(size_t particlesCount);
    ParticleManager();
    void onUpdate(Timestep timestep);

    void onMousePress()
    {
        m_isClicked = true;
    }

    void onMouseRelease()
    {
        m_isClicked = false;
    }

    void onMouseMove(float mousePosX, float mousePosY)
    {
        m_mousePosX = mousePosX;
        m_mousePosY = mousePosY;
    }

    void startSimulation()
    {
        m_isRunning = true;
    }

    void stopSimulation()
    {
        m_isRunning = false;
    }

    void toggleSimulation()
    {
        m_isRunning = !m_isRunning;
    }

private:
    void collisionCheck(Timestep timestep);
    void checkCollisionAgainstWalls(Particle& particle);
    bool overlapsWithAnyParticle(const glm::vec2& position, float radius);
    bool isOutisdeMap(const glm::vec2& position, float radius);
    bool isOutisdeMap(const Particle& particle);
    bool collisionHappened(const CollisionPair& collicurrentCollision);

private:
    std::vector<Particle> m_particles;
    std::vector<CollisionPair> m_collisionsBuffer;
    float timestep = 0.0f;
    bool m_isRunning = false;
    bool m_isClicked = false;
    float m_mousePosX = 0.0f;
    float m_mousePosY = 0.0f;
};


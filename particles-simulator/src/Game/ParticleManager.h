#pragma once
#include <random>
#include "Timestep.h"
#include "ApplicationModels.h"
#include "Game/Particle.h"
#include "Game/ParticleRenderer.h"
#include "Game/StateManager.h"

class ParticleManager
{
public:
    using CollisionPair = std::pair<Particle&, Particle&>;

    ParticleManager(AppConfig& config);
    ParticleManager() = default;

    void onUpdate(Timestep timestep);
    bool loadState();
    bool saveState();

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
        m_mousePosition.x = mousePosX;
        m_mousePosition.y = mousePosY;
    }

private:
    void collisionCheck();
    void detectCollisions();
    void resolveCollisions();
    void checkCollisionAgainstWalls(Particle& particle);
    bool overlapsWithAnyParticle(const glm::vec2& position, float radius);
    bool isOutisdeMap(const glm::vec2& position, float radius);
    bool isOutisdeMap(const Particle& particle);
    bool collisionHappened(const CollisionPair& collicurrentCollision);

    void dragParticle();
    AppConfig sanitizeConfig(const AppConfig& config);

private:
    std::vector<Particle> m_particles;
    std::vector<CollisionPair> m_collisionsBuffer;
    float timestep = 0.0f;
    bool m_isRunning = false;
    bool m_isClicked = false;
    glm::vec2 m_mousePosition = { 0.0f, 0.0f };
    std::string m_stateFilePath;
};


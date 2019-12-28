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
    void onUpdate(Timestep timestep, bool isMoving);
    //void start();
    //void stop();

private:
    void collisionCheck(Timestep timestep);
    void checkCollisionAgainstWalls(Particle& particle);
    bool isInsideAnyParticle(const glm::vec2& position, float radius);
    Particle* willBeInsideAnyParticle(const Particle& particle, Timestep timestep);
    bool isOutisdeMap(const glm::vec2& position, float radius);
    bool isOutisdeMap(const Particle& particle);
    bool collisionHappened(const CollisionPair& collicurrentCollision);

private:
    float timestep;
    std::vector<Particle> m_particles;
    std::vector<CollisionPair> m_collisionsBuffer;
};


#pragma once
#include "Particle.h"
#include "Timestep.h"

class ParticleManager
{
public:
    ParticleManager(size_t particlesCount);
    void onUpdate(Timestep timestep);
    void start();
    void stop();

private:
    void collisionCheck();

    float timestep;
    std::vector<Particle> m_particles;
};


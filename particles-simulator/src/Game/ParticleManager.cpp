#include "pch.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager(size_t particlesCount)
    : timestep(0.0f)
{
    m_particles.reserve(particlesCount);
}

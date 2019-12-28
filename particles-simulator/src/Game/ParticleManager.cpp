#include "pch.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager(size_t particlesCount)
    : timestep(0.0f)
{
    std::random_device rd;
    auto randEngine = std::default_random_engine(rd());
    auto distribPos = std::uniform_real_distribution<float>(-1.0f, 1.0f);
    auto distribSpeed = std::uniform_real_distribution<float>(-0.25f, 0.25f);
    auto distribRadius = std::uniform_real_distribution<float>(0.05f, 0.25f);

    m_particles.reserve(particlesCount);
    m_collisionsBuffer.reserve(particlesCount * (particlesCount - 1));

    for (size_t i = 0; i < particlesCount; i++)
    {
        glm::vec2 speed = glm::vec2(distribSpeed(randEngine), distribSpeed(randEngine));
        glm::vec2 pos;
        float radius;

        do
        {
            pos = glm::vec2(distribPos(randEngine), distribPos(randEngine));
            radius = distribRadius(randEngine);
        }
        while (isInsideAnyParticle(pos, radius) || isOutisdeMap(pos, radius));

        glm::vec4 color = glm::vec4((std::abs(speed.x) + std::abs(speed.y)) / 3.0f, std::abs(pos.x), std::abs(pos.y), 1.0f);
        m_particles.emplace_back(pos, speed, radius, color);
    }
}

ParticleManager::ParticleManager()
    : timestep(0.0f)
{
    //for tests
    size_t particlesCount = 3;
    m_particles.reserve(particlesCount);
    m_collisionsBuffer.reserve(particlesCount * (particlesCount - 1));

    m_particles.emplace_back(glm::vec2(0.5f, -0.5f), glm::vec2(-0.1f, 0.1f), 0.49f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    m_particles.emplace_back(glm::vec2(-0.5f, -0.5f), glm::vec2(0.1f, 0.1f), 0.49f, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
    m_particles.emplace_back(glm::vec2(0.0f, 0.4f), glm::vec2(0.0f, -0.2f), 0.52f, glm::vec4(1.0f, 0.2f, 0.2f, 1.0f));
}

void ParticleManager::onUpdate(Timestep timestep, bool isMoving)
{
    auto& renderer = ParticleRenderer::getInstance();
    if (isMoving) collisionCheck(timestep);

    for (auto& particle : m_particles)
    {
        if (isMoving) particle.onUpdate(timestep);
        renderer.render(particle);
    }
}

void ParticleManager::collisionCheck(Timestep timestep)
{
    m_collisionsBuffer.clear();

    //detect collisions
    for (auto& particle : m_particles)
    {
        checkCollisionAgainstWalls(particle);
        const auto particleID = particle.getID();
        const auto pos = particle.getPositionInNextFrame(timestep);

        for (auto& otherParticle : m_particles)
        {
            const auto otherParticleID = otherParticle.getID();
            if (particleID == otherParticleID) continue;

            const auto otherPos = otherParticle.getPositionInNextFrame(timestep);
            const float distanceBetweenCenters = glm::distance(pos, otherPos) - 0.005f;
            const float radiusSum = particle.getRadius() + otherParticle.getRadius();

            if (distanceBetweenCenters <= radiusSum)
            {
                CollisionPair currentCollision = particleID > otherParticleID ?
                    CollisionPair(otherParticle, particle) :
                    CollisionPair(particle, otherParticle);

                if (collisionHappened(currentCollision)) continue;
                m_collisionsBuffer.push_back(currentCollision);
            }
        }
    }

    for (const auto& collision : m_collisionsBuffer)
    {
        collision.first.onCollision(collision.second, m_particles, timestep);
    }
}

void ParticleManager::checkCollisionAgainstWalls(Particle& particle)
{
    const auto& position = particle.getPosition();
    const auto positionAbs = glm::vec2(std::abs(position.x), std::abs(position.y));
    const auto& speed = particle.getSpeed();
    float radius = particle.getRadius();

    if (positionAbs.x >= 1.0f - radius)
    {
        float difference = positionAbs.x + radius - 1.0f + 0.001f;
        float newX = position.x > 0.0f ? position.x - difference : position.x + difference;
        particle.setPosition(newX, position.y);
        particle.setSpeed(-speed.x, speed.y);
    }

    if (positionAbs.y >= 1.0f - radius)
    {
        float difference = positionAbs.y + radius - 1.0f + 0.001f;
        float newY = position.y > 0.0f ? position.y - difference : position.y + difference;
        particle.setPosition(position.x, newY);
        particle.setSpeed(speed.x, -speed.y);
    }
}

bool ParticleManager::isInsideAnyParticle(const glm::vec2& position, float radius)
{
    for (const auto& particle : m_particles)
    {
        if (glm::distance(position, particle.getPosition()) - radius <= particle.getRadius() + 0.005f)
            return true;
    }
    return false;
}

Particle* ParticleManager::willBeInsideAnyParticle(const Particle& particle, Timestep timestep)
{
    glm::vec2 position = particle.getPositionInNextFrame(timestep);
    float radius = particle.getRadius();
    for (auto& p : m_particles)
    {
        if (p.getID() == particle.getID()) continue;
        if (glm::distance(position, p.getPositionInNextFrame(timestep)) - radius <= p.getRadius()) return &p;
    }
    return nullptr;
}

bool ParticleManager::isOutisdeMap(const glm::vec2& position, float radius)
{
    return std::abs(position.x) > 1.0f - radius - 0.005f || std::abs(position.y) > 1.0f - radius - 0.005f;
}

bool ParticleManager::isOutisdeMap(const Particle& particle)
{
    const auto position = particle.getPosition();
    const float radius = particle.getRadius();
    return std::abs(position.x) > 1.0f - radius - 0.005f || std::abs(position.y) > 1.0f - radius - 0.005f;
}

bool ParticleManager::collisionHappened(const CollisionPair& collicurrentCollision)
{
    for (const auto& collision : m_collisionsBuffer)
        if (collision == collicurrentCollision) return true;
    return false;
}

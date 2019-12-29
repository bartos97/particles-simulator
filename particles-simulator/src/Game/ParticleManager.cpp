#include "pch.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager(size_t particlesCount)
{
    static const size_t MAX_ATTEMPTS = 10000;

    m_particles.reserve(particlesCount);
    m_collisionsBuffer.reserve(particlesCount * (particlesCount - 1));

    std::random_device rd;
    auto randEngine = std::default_random_engine(rd());
    auto distribPos = std::uniform_real_distribution<float>(-1.0f, 1.0f);
    auto distribSpeed = std::uniform_real_distribution<float>(-0.5f, 0.5f);
    auto distribRadius = std::uniform_real_distribution<float>(0.025f, 0.2f);


    for (size_t i = 0; i < particlesCount; i++)
    {
        glm::vec2 speed = glm::vec2(distribSpeed(randEngine), distribSpeed(randEngine));
        glm::vec2 pos;
        float radius;
        size_t attemptsCounter = 0;

        do
        {
            pos = glm::vec2(distribPos(randEngine), distribPos(randEngine));
            radius = distribRadius(randEngine);
            attemptsCounter++;
        }
        while (attemptsCounter < MAX_ATTEMPTS && (overlapsWithAnyParticle(pos, radius) || isOutisdeMap(pos, radius)));

        if (attemptsCounter >= MAX_ATTEMPTS)
        {
            PS_INFO("Can't find place for more particles, only %llu will be made", i + 1);
            return;
        }
        else
        {
            glm::vec4 color = glm::vec4(std::abs(speed.x) + std::abs(speed.y), std::abs(pos.x), std::abs(pos.y), 1.0f);
            m_particles.emplace_back(pos, speed, radius, color);
        }
    }
}

ParticleManager::ParticleManager()
{
    //for tests
    size_t particlesCount = 2;
    m_particles.reserve(particlesCount);
    m_collisionsBuffer.reserve(particlesCount * (particlesCount - 1));

    m_particles.emplace_back(glm::vec2(-0.5f, 0.5f), glm::vec2(0.1f, -0.1f), 0.1f, glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));
    //m_particles.emplace_back(glm::vec2(0.5f, 0.5f), glm::vec2(-0.25f, -0.25f), 0.1f, glm::vec4(0.8f, 0.8f, 0.8f, 1.0f));
    m_particles.emplace_back(glm::vec2(0.5f, -0.5f), glm::vec2(-0.1f, 0.1f), 0.5f, glm::vec4(0.4f, 0.4f, 0.4f, 1.0f));
    //m_particles.emplace_back(glm::vec2(-0.5f, -0.5f), glm::vec2(0.1f, 0.1f), 0.49f, glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));
}

void ParticleManager::onUpdate(Timestep timestep)
{
#ifdef PS_DEBUG
    static int clickedParticle;
    if (m_isClicked)
    {
        for (auto& particle : m_particles)
        {
            int ID = particle.getID();
            if (clickedParticle == -1 && particle.isInside(m_mousePosX, m_mousePosY))
            {
                clickedParticle = ID;
                particle.setPosition(m_mousePosX, m_mousePosY);
                break;
            }
            else if (clickedParticle == ID)
            {
                particle.setPosition(m_mousePosX, m_mousePosY);
            }
        }
    }
    else clickedParticle = -1;
#endif // PS_DEBUG

    for (auto& particle : m_particles)
    {
        particle.onUpdate(timestep, m_isRunning);
    }
    
    if (m_isRunning) collisionCheck(timestep);

    auto& renderer = ParticleRenderer::getInstance();
    for (auto& particle : m_particles)
    {
        renderer.render(particle);
    }
}

void ParticleManager::collisionCheck(Timestep timestep)
{
    m_collisionsBuffer.clear();

    //detect collisions and displace particles so they don't overlap
    for (auto& particle : m_particles)
    {
        checkCollisionAgainstWalls(particle);
        const auto particleID = particle.getID();
        const auto& pos = particle.getPosition();

        for (auto& otherParticle : m_particles)
        {
            const auto otherParticleID = otherParticle.getID();
            if (particleID == otherParticleID) continue;

            const auto& otherPos = otherParticle.getPosition();
            const float distanceBetweenCenters = glm::distance(pos, otherPos);
            const float radiusSum = particle.getRadius() + otherParticle.getRadius();

            if (distanceBetweenCenters <= radiusSum)
            {
                CollisionPair currentCollision = particleID > otherParticleID ?
                    CollisionPair(otherParticle, particle) :
                    CollisionPair(particle, otherParticle);
                if (collisionHappened(currentCollision)) continue;
                m_collisionsBuffer.push_back(currentCollision);

                float overlap = 0.5f * (radiusSum - distanceBetweenCenters);
                glm::vec2 displacement = pos - otherPos;
                displacement = overlap * displacement / distanceBetweenCenters;
                
                particle.setPosition(pos + displacement);
                otherParticle.setPosition(otherPos - displacement);
            }
        }
    }

    //resolve collisions
    for (const auto& collision : m_collisionsBuffer)
    {
        auto& first = collision.first;
        auto& second = collision.second;

        float firstMass = first.getMass();
        float secondMass = second.getMass();
        float massesSum =  firstMass + secondMass;
        const glm::vec2& firstSpeed = first.getSpeed();
        const glm::vec2& secondSpeed = second.getSpeed();

        const glm::vec2 normal = glm::normalize(first.getPosition() - second.getPosition());
        const glm::vec2 tangent = glm::vec2(-normal.y, normal.x);

        float firstTangentDot = glm::dot(firstSpeed, tangent);
        float secondTangentDot = glm::dot(secondSpeed, tangent);
        float firstNormalDot = glm::dot(firstSpeed, normal);
        float secondNormalDot = glm::dot(secondSpeed, normal);

        // Conservation of momentum in 1D
        float firstMomentum = ((firstMass - secondMass) * firstNormalDot + 2.0f * secondMass * secondNormalDot) / massesSum;
        float secondMomentum = (2.0f * firstMass * firstNormalDot + (secondMass - firstMass) * secondNormalDot) / massesSum;

        first.setSpeed(tangent * firstTangentDot + normal * firstMomentum);
        second.setSpeed(tangent * secondTangentDot + normal * secondMomentum);
        first.onCollision();
        second.onCollision();
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

bool ParticleManager::overlapsWithAnyParticle(const glm::vec2& position, float radius)
{
    for (const auto& particle : m_particles)
    {
        if (glm::distance(position, particle.getPosition()) - radius <= particle.getRadius())
            return true;
    }
    return false;
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
    {
        if (collision == collicurrentCollision) return true;
    }        
    return false;
}
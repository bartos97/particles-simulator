#include "pch.h"
#include "ParticleManager.h"

ParticleManager::ParticleManager(AppConfig& config)
    : m_stateFilePath(Core::PROJECT_ABS_PATH + config.path)
{
    config = sanitizeConfig(config);

    if (!config.numSet && loadState())
    {
        return;
    }

    PS_INFO("Generating random simulation state");
    m_particles.reserve(config.num);
    m_collisionsBuffer.reserve(config.num * (config.num - 1));

    std::random_device rd;
    auto randEngine = std::default_random_engine(rd());
    auto distribPos = std::uniform_real_distribution<float>(-1.0f, 1.0f);
    auto distribSpeed = std::uniform_real_distribution<float>(config.speedFrom, config.speedTo);
    auto distribRadius = std::uniform_real_distribution<float>(config.radiusFrom, config.radiusTo);


    static const size_t MAX_ATTEMPTS = 10000;
    for (size_t i = 0; i < config.num; i++)
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
            float red = (std::abs(speed.x) + std::abs(speed.y)) / 2.0f * config.speedTo;
            glm::vec4 color = glm::vec4(red, std::abs(pos.x), std::abs(pos.y), 1.0f);
            m_particles.emplace_back(pos, speed, radius, color);
        }
    }
}

void ParticleManager::onUpdate(Timestep timestep)
{
#ifdef PS_DEBUG
    dragParticle();
#endif // PS_DEBUG
    
    static auto& renderer = ParticleRenderer::getInstance();

    for (auto& particle : m_particles)
    {
        particle.onUpdate(timestep, m_isRunning);
    }
    
    if (m_isRunning) collisionCheck();

    for (auto& particle : m_particles)
    {
        renderer.render(particle, m_mousePosition);
    }
}

bool ParticleManager::loadState()
{
    bool wasStopped = !m_isRunning;
    stopSimulation();

    auto& manager = StateManager::getInstance();
    if (!manager.loadFromFile(m_stateFilePath))
    {
        if (manager.getStates().size())
        {
            PS_INFO("Latest saved simulation state will be loaded");
        }
        else
        {
            PS_INFO("Preserving current simulation state");
            return false;
        }
    }

    const auto& states = manager.getStates();
    size_t numOfStates = states.size();
    size_t numOfParticles = m_particles.size();

    if (numOfStates >= numOfParticles)
    {
        unsigned int i = 0;
        for (; i < numOfParticles; i++)
        {
            m_particles[i].setState(states[i]);
        }

        if (numOfStates > numOfParticles)
        {
            m_particles.reserve(numOfStates);
            m_collisionsBuffer.reserve(numOfStates * (numOfStates - 1));
            for (; i < numOfStates; i++)
            {
                m_particles.emplace_back(states[i]);
            }
        }
    }
    else
    {
        m_particles.resize(numOfStates);
        for (unsigned int i = 0; i < numOfStates; i++)
        {
            m_particles[i].setState(states[i]);
        }
        m_collisionsBuffer.clear();
        m_collisionsBuffer.reserve(numOfStates * (numOfStates - 1));
    }

    if (!wasStopped)
        startSimulation();

    PS_INFO("Simulation state recovered with loaded data");
    return true;
}

bool ParticleManager::saveState()
{
    bool wasStopped = !m_isRunning;
    stopSimulation();

    auto& manager = StateManager::getInstance();
    bool retVal = true;
    manager.setStates(m_particles);
    
    if (!manager.saveToFile(m_stateFilePath))
    {
        PS_INFO("Latest saved simulation state will be preserved");
        retVal = false;
    }

    if (!wasStopped)
        startSimulation();
    return retVal;
}

void ParticleManager::collisionCheck()
{
    detectCollisions();
    resolveCollisions();
}

void ParticleManager::detectCollisions()
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
}

void ParticleManager::resolveCollisions()
{
    for (const auto& collision : m_collisionsBuffer)
    {
        auto& first = collision.first;
        auto& second = collision.second;

        float firstMass = first.getMass();
        float secondMass = second.getMass();
        float massesSum = firstMass + secondMass;
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

void ParticleManager::dragParticle()
{
    static int clickedParticle;
    if (m_isClicked)
    {
        for (auto& particle : m_particles)
        {
            int ID = particle.getID();
            if (clickedParticle == -1 && particle.isInside(m_mousePosition.x, m_mousePosition.y))
            {
                clickedParticle = ID;
                particle.setPosition(m_mousePosition.x, m_mousePosition.y);
                break;
            }
            else if (clickedParticle == ID)
            {
                particle.setPosition(m_mousePosition.x, m_mousePosition.y);
            }
        }
    }
    else clickedParticle = -1;
}

AppConfig ParticleManager::sanitizeConfig(const AppConfig& config)
{
    float speedTo = AppConfig::DEFAULTS.speedTo;
    float radiusFrom = AppConfig::DEFAULTS.radiusFrom;
    float radiusTo = AppConfig::DEFAULTS.radiusTo;
    bool changed = false;

    if (config.speedTo > 2.0f || config.speedTo <= config.speedFrom)
    {
        PS_INFO("Param --speedTo mustn't be greater than 2.0 or smaller than --speedFrom (%f by default).\nUsing default value = %f for --speedTo", 
                AppConfig::DEFAULTS.speedFrom, 
                AppConfig::DEFAULTS.speedTo);
        changed = true;
    }

    if (config.radiusFrom < AppConfig::DEFAULTS.radiusFrom)
    {
        PS_INFO("Param --radiusFrom mustn't be smaller than %f.\nUsing default value = %f for --radiusFrom",
                AppConfig::DEFAULTS.radiusFrom,
                AppConfig::DEFAULTS.radiusFrom);
        changed = true;
    }

    if (config.radiusTo > 0.5f || config.radiusTo <= config.radiusFrom)
    {
        PS_INFO("Param --radiusTo mustn't be greater than 0.5 or smaller than --radiusFrom (%f by default).\nUsing default value = %f for --radiusTo",
                AppConfig::DEFAULTS.radiusFrom,
                AppConfig::DEFAULTS.radiusTo);
        changed = true;
    }

    if (!changed)
        return config;

    return { config.num, config.numSet, config.path, config.pathSet, config.speedFrom, speedTo, radiusFrom, radiusTo };
}

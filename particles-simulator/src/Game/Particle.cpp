#include "pch.h"
#include "Particle.h"

const float Particle::COLLISION_COLOR_ANIMATION_TIME = 0.5f;
const glm::vec4 Particle::COLLISION_COLOR = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);

Particle::Particle(const glm::vec2& position, const glm::vec2& speed, float radius, const glm::vec4& color)
    : m_position(position), m_speed(speed), m_radius(radius), m_area(float(M_PI * m_radius * m_radius)),
      m_color(color), m_defaultColor(color), m_collisionColorComplement(COLLISION_COLOR - color)
{
    static unsigned int counter;
    m_ID = counter;
    counter++;;
}

Particle::Particle(const ParticleState& state)
    : Particle()
{
    setState(state);
}

void Particle::onUpdate(Timestep ts, bool updatePosition)
{
    updateColor(ts);
    m_timeSinceCollision += ts;
    if (updatePosition)
    {
        m_position += m_speed * float(ts);
    }
}

void Particle::onCollision()
{
    m_timeSinceCollision = 0.0f;
}

void Particle::setState(const ParticleState& state)
{
    static const char DELIMITER = ';';
    const std::string& data = state.getState();
    float extractedData[8] = { 0.0f };

    size_t pos = data.find_first_of(DELIMITER);
    size_t lastPos = 0;
    unsigned int i = 0;
    while (true)
    {
        bool assertCond = i <= 8;
        if (!assertCond)
        {
        #ifdef PS_DEBUG
            PS_ASSERT(assertCond, "Too many values in ParticleState data");
        #else
            PS_ERROR_INFO("Error has occured when recovering simulation state. Incorrect data in state file:\nToo many values per one particle.\n");
        #endif // PS_DEBUG
            break;
        }

        extractedData[i] = std::atof(data.substr(lastPos, pos).c_str());

        if (pos == std::string::npos)
        {
            break;
        }

        lastPos = pos + 1;
        pos = data.find_first_of(DELIMITER, pos + 1);
        i++;
    }
    
    m_position.x = extractedData[0];
    m_position.y = extractedData[1];
    m_speed.x = extractedData[2];
    m_speed.y = extractedData[3];
    m_radius = extractedData[4];
    m_color.x = extractedData[5];
    m_color.y = extractedData[6];
    m_color.z = extractedData[7];

    m_defaultColor = m_color;
    m_collisionColorComplement = glm::vec4(COLLISION_COLOR - m_color);
}

ParticleState Particle::createState() const
{
    std::stringstream dataStream;
    dataStream
        << m_position.x     << ';' 
        << m_position.y     << ';'
        << m_speed.x        << ';' 
        << m_speed.y        << ';'
        << m_radius         << ';'
        << m_defaultColor.x << ';' 
        << m_defaultColor.y << ';' 
        << m_defaultColor.z << '\n';

    return ParticleState(dataStream.str());
}

void Particle::updateColor(Timestep ts)
{
    if (m_timeSinceCollision <= COLLISION_COLOR_ANIMATION_TIME)
    {
        auto colorDelta = m_collisionColorComplement * (float(ts) / COLLISION_COLOR_ANIMATION_TIME / 0.5f);
        if (m_timeSinceCollision <= COLLISION_COLOR_ANIMATION_TIME / 2.0f)
        {
            if (m_color.x <= 1.0f) m_color += colorDelta;
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
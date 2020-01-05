#include "pch.h"
#include "StateManager.h"
#include <fstream>
#include <algorithm>

StateManager& StateManager::getInstance()
{
    static StateManager instance;
    return instance;
}

bool StateManager::loadFromFile(const std::string& filePath)
{
    std::ifstream fileStream(filePath, std::ios::in);

    if (!fileStream.is_open())
    {
        PS_INFO("Could not open simulation state file: %s", filePath.c_str());
        return false;
    }

    size_t numOfLines = std::count(std::istreambuf_iterator<char>(fileStream),
                                 std::istreambuf_iterator<char>(), '\n');
    if (numOfLines == 0)
    {
        PS_INFO("Simulation state file is empty: %s", filePath.c_str());
        return false;
    }


    std::vector<ParticleState> buffer;
    m_states.clear();
    m_states.reserve(numOfLines);
    buffer.reserve(numOfLines);
    fileStream.seekg(0);
    bool fail = false;
    bool bad = false;

    for (std::string line; std::getline(fileStream, line); )
    {
        buffer.emplace_back(line);
        fail = fileStream.fail();
        bad = fileStream.bad();
    }

    if (fail || bad)
    {
        PS_INFO("Failed when reading from file: %s", filePath.c_str());
        fileStream.close();
        return false;
    }

    m_states.swap(buffer);
    PS_INFO("Simulation state read from file successfully");
    return true;
}

bool StateManager::saveToFile(const std::string& filePath) const
{
    std::ofstream fileStream(filePath, std::ios::out);

    if (!fileStream)
    {
        PS_INFO("Could not open simulation state file: %s", filePath.c_str());
        return false;
    }

    for (const auto& state : m_states)
    {
        fileStream << state.getState() << '\n';
    }

    if (!fileStream.good())
    {
        PS_INFO("Failed when writing to file: %s", filePath.c_str());
        fileStream.close();
        return false;
    }


    PS_INFO("Simulation state saved to file successfully");
    return true;
}

void StateManager::setStates(const std::vector<Particle>& particles)
{
    m_states.clear();
    m_states.reserve(particles.size());

    for (const auto& particle : particles)
    {
        m_states.push_back(particle.createState());
    }
}

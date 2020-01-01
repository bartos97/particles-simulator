#pragma once
#include "Game/ParticleState.h"
#include "Game/Particle.h"

class StateManager
{
public:
    static StateManager& getInstance();
    StateManager(const StateManager&) = delete;
    void operator=(const StateManager&) = delete;

    bool loadFromFile(const std::string& filePath);
    bool saveToFile(const std::string& filePath) const;

    void setStates(const std::vector<Particle>& particles);

    const std::vector<ParticleState>& getStates() const
    {
        return m_states;
    }

private:
    StateManager() = default;

private:
    std::vector<ParticleState> m_states;
};


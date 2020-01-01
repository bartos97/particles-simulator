#pragma once
#include "pch.h"

class Particle;

/*
 * `Memento` in Memento Design Pattern
 * data pattern: position.x;position.y;speed.x;speed.y;radius;color.r;color.g;color.b\n
 */
class ParticleState
{
public:
    ParticleState(std::string particleData)
        : data(particleData)
    {}

    ~ParticleState() = default;

    const std::string& getState() const
    {
        return data;
    }

private:
    friend class Particle;

    void setState(std::string particleData)
    {
        data = particleData;
    }

private:
    std::string data;
};
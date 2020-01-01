#pragma once

struct AppConfig
{
    static const AppConfig DEFAULTS;

    size_t num = AppConfig::DEFAULTS.num; //number of particles to simulate
    bool numSet = AppConfig::DEFAULTS.numSet;
    std::string path = AppConfig::DEFAULTS.path; //path (relative to project roor) to file to contain saved simulation state
    bool pathSet = AppConfig::DEFAULTS.pathSet;
    float speedFrom = AppConfig::DEFAULTS.speedFrom;
    float speedTo = AppConfig::DEFAULTS.speedTo;
    float radiusFrom = AppConfig::DEFAULTS.radiusFrom;
    float radiusTo = AppConfig::DEFAULTS.radiusTo;
};

struct AppExecParam
{
    const char* key;
    std::function<bool(const char*)> callback;
};
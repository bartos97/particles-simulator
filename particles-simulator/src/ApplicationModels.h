#pragma once

struct AppConfig
{
    size_t num = 10; //number of particles to simulate
    std::string path = "state.txt"; //path (relative to project roor) to file to contain saved simulation state
};

struct AppExecParam
{
    const char* key;
    std::function<bool(const char*)> callback;
};

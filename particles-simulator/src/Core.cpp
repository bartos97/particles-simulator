#include "pch.h"
#include "Core.h"
#include <filesystem>


const std::string Core::PROJECT_ABS_PATH = Core::getPath();

std::string Core::stripProjectPath(const std::string& filePath)
{
    auto pos = filePath.find(Core::PROJECT_ABS_PATH);
    if (pos == std::string::npos) return filePath;

    size_t start =  pos + Core::PROJECT_ABS_PATH.size();
    return filePath.substr(start);
}

std::string Core::stripProjectPath(const char* filePath)
{
    return Core::stripProjectPath(std::string(filePath));
}

std::string Core::getPath()
{
    std::string path = std::filesystem::current_path().string();

    std::string::size_type pos;
    std::string::size_type tmpPos = 0;
    while ((tmpPos = path.find("bin", tmpPos)) != std::string::npos)
    {
        pos = tmpPos;
        tmpPos++;
    }
    path = path.substr(0, pos);

    char lastChar = path.at(path.size() - 1);
    if (!(lastChar == '\\' || lastChar == '/'))
    {
    #ifdef PS_SYSTEM_WINDOWS
        path += '\\';
    #else
        path += '/';
    #endif // PS_SYSTEM_WINDOWS
    }

    path += TOSTRING(PS_PROJ_NAME);
#ifdef PS_SYSTEM_WINDOWS
    path += '\\';
#else
    path += '/';
#endif // PS_SYSTEM_WINDOWS
    return path;
}

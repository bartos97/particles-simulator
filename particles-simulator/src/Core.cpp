#include "pch.h"
#include "Core.h"
#include <filesystem>


const std::string Core::PROJECT_ABS_PATH = Core::getPath();

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
        path += '/';
    }

    path += TOSTRING(PS_PROJ_NAME);
    path += '/';
    PS_LOG("Project path: %s", path.c_str());
    return path;
}

#pragma once

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

#ifdef PS_DEBUG
    #define PS_ENABLE_LOG
    #define PS_ENABLE_ASSERT
#endif // PS_DEBUG


#ifdef PS_RELEASE
    //#define PS_FULLSCREEN
#endif // PS_RELEASE


class Core
{
public:
    static const std::string PROJECT_ABS_PATH;
    static std::string stripProjectPath(const std::string& filePath);
    static std::string stripProjectPath(const char* filePath);

private:
    Core() = default;
    static std::string getPath();
};


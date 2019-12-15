#include "pch.h"
#include "Shader.h"
#include <fstream>
#include <sstream>


const Shader* Shader::m_currentlyBound = nullptr;


Shader::Shader(const char* vertexShaderPath, const char* fragmentShaderPath)
    : m_id(0)
{
    PS_LOG("Shader object constructed");
    assignData(vertexShaderPath, fragmentShaderPath);
    m_isDataAssigned = true;
}


Shader::Shader()
    : m_isDataAssigned(false), m_id(0)
{
    PS_LOG("Shader object constructed without data assignment!");
}


Shader::~Shader()
{
    GL_CALL(glDeleteProgram(m_id));
    PS_LOG("Shader #%d deleted", m_id);
}


void Shader::assignData(const char* vertexShaderPath, const char* fragmentShaderPath)
{
    std::string vertexStr;
    std::string fragmentStr;
    readFiles(vertexShaderPath, fragmentShaderPath, vertexStr, fragmentStr);
    createShaderProgram(vertexStr, fragmentStr);
    m_isDataAssigned = true;
    PS_LOG("Shader #%d data assigned", m_id);
}


void Shader::bind() const
{
    PS_ASSERT(m_isDataAssigned, "Tring to bind Shader without data assigned!");
    if (Shader::m_currentlyBound != this)
    {
        GL_CALL(glUseProgram(m_id));
        Shader::m_currentlyBound = this;
        PS_LOG("Shader #%d is now bound", m_id);
    }
}


void Shader::unbind() const
{
    GL_CALL(glUseProgram(0));
    Shader::m_currentlyBound = nullptr;
    PS_LOG("No Shader bound.");
}


void Shader::setUniform(const std::string& name, float x)
{
    int location = glGetUniformLocation(m_id, name.c_str());
    glUniform1f(location, x);
}

void Shader::setUniform(const std::string& name, bool x)
{
    int location = glGetUniformLocation(m_id, name.c_str());
    glUniform1i(location, x);
}

void Shader::setUniform(const std::string& name, int x)
{
    int location = glGetUniformLocation(m_id, name.c_str());
    glUniform1i(location, x);
}

void Shader::setUniform(const std::string& name, float x, float y, float z, float w)
{
    int location = glGetUniformLocation(m_id, name.c_str());
    glUniform4f(location, x, y, z, w);
}

void Shader::setUniform(const std::string& name, const glm::vec4& vec)
{
    setUniform(name, vec.x, vec.y, vec.z, vec.w);
}

void Shader::setUniform(const std::string& name, float x, float y, float z)
{
    int location = glGetUniformLocation(m_id, name.c_str());
    glUniform3f(location, x, y, z);
}

void Shader::setUniform(const std::string& name, const glm::vec3& vec)
{
    setUniform(name, vec.x, vec.y, vec.z);
}

void Shader::setUniform(const std::string& name, float x, float y)
{
    int location = glGetUniformLocation(m_id, name.c_str());
    glUniform2f(location, x, y);
}

void Shader::setUniform(const std::string& name, const glm::vec2& vec)
{
    setUniform(name, vec.x, vec.y);
}

void Shader::setUniform(const std::string& name, const glm::mat4& mat)
{
    int location = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::setUniform(const std::string& name, const glm::mat3& mat)
{
    int location = glGetUniformLocation(m_id, name.c_str());
    glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}


void Shader::readFiles(const char* vertexShaderPath,
                       const char* fragmentShaderPath,
                       std::string& vertexStr,
                       std::string& fragmentStr)
{
    std::ifstream vertexFile;
    std::ifstream fragmentFile;
    vertexFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);
    fragmentFile.exceptions(std::ifstream::badbit | std::ifstream::failbit);

    try
    {
        std::stringstream vertexStream;
        vertexFile.open(vertexShaderPath);
        vertexStream << vertexFile.rdbuf();
        vertexFile.close();
        vertexStr = vertexStream.str();

        std::stringstream fragmentStream;
        fragmentFile.open(fragmentShaderPath);
        fragmentStream << fragmentFile.rdbuf();
        fragmentFile.close();
        fragmentStr = fragmentStream.str();
    }
    catch (const std::exception & e)
    {
        PS_ASSERT(false, "Failed with some shader file!\n%s", e.what());
    }
}


void Shader::createShaderProgram(std::string& vertexStr, std::string& fragmentStr)
{
    unsigned int vertexShader = compileSingleShader(vertexStr.c_str(), GL_VERTEX_SHADER);
    unsigned int fragmentShader = compileSingleShader(fragmentStr.c_str(), GL_FRAGMENT_SHADER);

    int success;
    GL_CALL(m_id = glCreateProgram());
    GL_CALL(glAttachShader(m_id, vertexShader));
    GL_CALL(glAttachShader(m_id, fragmentShader));
    GL_CALL(glLinkProgram(m_id));

    glGetProgramiv(m_id, GL_LINK_STATUS, &success);
    if (!success)
    {
        char errorInfo[512];
        glGetProgramInfoLog(m_id, 512, NULL, errorInfo);
        PS_ASSERT(success, "%s", errorInfo);
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}


unsigned int Shader::compileSingleShader(const char* shaderStr, GLenum shaderEnum)
{
    int success;

    GL_CALL(unsigned int shader = glCreateShader(shaderEnum));
    GL_CALL(glShaderSource(shader, 1, &shaderStr, NULL));
    GL_CALL(glCompileShader(shader));

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        char errorInfo[512];
        glGetShaderInfoLog(shader, 512, NULL, errorInfo);
        PS_ASSERT(success, "%s", errorInfo);
    }

    return shader;
}
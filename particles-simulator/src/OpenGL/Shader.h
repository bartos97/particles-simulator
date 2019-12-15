#pragma once


class Shader
{
public:
    Shader(const char* vertexShaderPath, const char* fragmentShaderPath);
    Shader();

    ~Shader();

    void assignData(const char* vertexShaderPath, const char* fragmentShaderPath);

    void bind() const;
    void unbind() const;

    void setUniform(const std::string& name, float x);
    void setUniform(const std::string& name, bool x);
    void setUniform(const std::string& name, int x);

    void setUniform(const std::string& name, float x, float y, float z, float w);
    void setUniform(const std::string& name, const glm::vec4& vec);

    void setUniform(const std::string& name, float x, float y, float z);
    void setUniform(const std::string& name, const glm::vec3& vec);

    void setUniform(const std::string& name, float x, float y);
    void setUniform(const std::string& name, const glm::vec2& vec);

    void setUniform(const std::string& name, const glm::mat4& mat);
    void setUniform(const std::string& name, const glm::mat3& mat);

private:
    static const Shader* m_currentlyBound;
    unsigned int m_id;
    bool m_isDataAssigned;

    void readFiles(const char* vertexShaderPath,
                   const char* fragmentShaderPath,
                   std::string& vertexStr,
                   std::string& fragmentStr);
    void createShaderProgram(std::string& vertexStr, std::string& fragmentStr);
    unsigned int compileSingleShader(const char* shaderStr, GLenum shaderEnum);
};

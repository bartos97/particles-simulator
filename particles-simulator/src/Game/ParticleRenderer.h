#pragma once
#include "Particle.h"
#include "OpenGL/Renderer.h"

class ParticleRenderer
{
public:
    static ParticleRenderer& getInstance();
    ParticleRenderer(const ParticleRenderer&) = delete;
    void operator=(const ParticleRenderer&) = delete;

    void render(const Particle& particle);
    void setMousePosition(float x, float y);

private:
    ParticleRenderer();

    static const std::string s_vertexShaderPath;
    static const std::string s_fragmentShaderPath;
    static const float s_vertices[8];
    static const unsigned int s_indecies[6];

    Shader m_shader;
    VertexBuffer m_vertexBuffer;
    ElementBuffer m_elementBuffer;
    BufferLayout m_bufferLayout;
    VertexArray m_vertexArray;

    glm::vec2 m_mousePosition = { 0.0f, 0.0f };
};

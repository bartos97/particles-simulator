#pragma once
#include "Particle.h"
#include "OpenGL/Renderer.h"

class ParticleRenderer
{
public:
    static ParticleRenderer& getInstance();
    ParticleRenderer(const ParticleRenderer&) = delete;
    void operator=(const ParticleRenderer&) = delete;

    void render(const Particle& particle, const glm::vec2& mousePos);

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
};

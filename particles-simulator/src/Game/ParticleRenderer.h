#pragma once
#include "Particle.h"
#include "OpenGL/Renderer.h"

class ParticleRenderer
{
public:
    ParticleRenderer(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> va);
    void render(const Particle& particle);

private:
    std::shared_ptr<Shader> m_shader;
    std::shared_ptr<VertexArray> m_vertexArray;
};

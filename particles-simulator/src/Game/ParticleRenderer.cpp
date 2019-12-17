#include "pch.h"
#include "ParticleRenderer.h"

ParticleRenderer::ParticleRenderer(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> va)
    : m_shader(shader), m_vertexArray(va)
{}

void ParticleRenderer::render(const Particle& particle)
{
    float r = particle.getRadius();
    float x = particle.getPosition().x;
    float y = particle.getPosition().y;

    glm::mat4 transformMatrix = glm::mat4(1.0f);
    transformMatrix = glm::scale(glm::vec3(r, r, 1.0f ));
    transformMatrix = glm::translate(transformMatrix, glm::vec3(x * (1.0f / r), y * (1.0f / r), 1.0f));

    m_shader->setUniform("u_transformation", transformMatrix);
    m_shader->setUniform("u_radius", particle.getRadius());
    m_shader->setUniform("u_center", particle.getPosition());
    Renderer::draw(m_vertexArray, m_shader);
}
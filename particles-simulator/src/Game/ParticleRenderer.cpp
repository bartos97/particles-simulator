#include "pch.h"
#include "ParticleRenderer.h"

const std::string ParticleRenderer::s_vertexShaderPath = Core::PROJECT_ABS_PATH + "res/Shaders/circle.vert.glsl";
const std::string ParticleRenderer::s_fragmentShaderPath = Core::PROJECT_ABS_PATH + "res/Shaders/circle.frag.glsl";

const float ParticleRenderer::s_vertices[8] = {
    -1.0f,  1.0f,
     1.0f,  1.0f,
     1.0f, -1.0f,
    -1.0f, -1.0f
};

const unsigned int ParticleRenderer::s_indecies[6] = {
    0, 1, 2,
    2, 3, 0
};

ParticleRenderer& ParticleRenderer::getInstance()
{
    static ParticleRenderer instance;
    return instance;
}

ParticleRenderer::ParticleRenderer()
    : m_shader(ParticleRenderer::s_vertexShaderPath.c_str(), ParticleRenderer::s_fragmentShaderPath.c_str()),
    m_vertexBuffer(sizeof(float) * 8, ParticleRenderer::s_vertices),
    m_elementBuffer(6, ParticleRenderer::s_indecies)
{
    m_bufferLayout.add<float>(2);
    m_vertexArray.assignData(m_vertexBuffer, m_elementBuffer, m_bufferLayout);
}

void ParticleRenderer::render(const Particle& particle)
{
    float r = particle.getRadius();
    float x = particle.getPosition().x;
    float y = particle.getPosition().y;

    glm::mat4 transformMatrix = glm::mat4(1.0f);
    transformMatrix = glm::scale(glm::vec3(r, r, 1.0f ));
    transformMatrix = glm::translate(transformMatrix, glm::vec3(x * (1.0f / r), y * (1.0f / r), 1.0f));

    m_shader.setUniform("u_transformation", transformMatrix);
    m_shader.setUniform("u_radius", particle.getRadius());
    m_shader.setUniform("u_center", particle.getPosition());
    m_shader.setUniform("u_color", particle.getColor());
    m_shader.setUniform("u_mousePosition", this->m_mousePosition);
    Renderer::draw(m_vertexArray, m_shader);
}

void ParticleRenderer::setMousePosition(float x, float y)
{
    this->m_mousePosition.x = x;
    this->m_mousePosition.y = y;
}

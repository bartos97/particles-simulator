#pragma once

#include "OpenGL/VertexArray.h"
#include "OpenGL/Shader.h"


class Renderer
{
public:
    static void draw(const VertexArray& va, const Shader& shader);
    static void draw(std::shared_ptr<VertexArray> va, std::shared_ptr<Shader> shader);
    static void clearScreen();

    static void setMode(GLenum newMode);
};
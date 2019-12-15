#pragma once
#include "OpenGL/GLbuffer.h"

class VertexBuffer : public GLbuffer
{
public:
    /**
     * Generates and binds Vertex Buffer Object
     * @param size - size in bytes of given data
     * @param data - self-explanatory
     * @param usagePattern - specifies the expected usage pattern of the data store
     * by default it is GL_STATIC_DRAW
     */
    VertexBuffer(unsigned int size, const void* data, GLenum usagePattern = GL_STATIC_DRAW);
    VertexBuffer()
        : GLbuffer()
    {}

    virtual void assignData(unsigned int size, const void* data, GLenum usagePattern = GL_STATIC_DRAW) override;

    virtual void bind() const override;
    static void unbindAll();
};


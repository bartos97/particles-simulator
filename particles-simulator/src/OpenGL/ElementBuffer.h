#pragma once
#include "OpenGL/GLbuffer.h"

class ElementBuffer : public GLbuffer
{
public:
    /**
     * Generates and binds Element Buffer Object (or Index Buffer, if you like)
     * @param count - how many items there is in data
     * @param data - self-explanatory
     * @param usagePattern - specifies the expected usage pattern of the data store
     * by default it is GL_STATIC_DRAW
     */
    ElementBuffer(unsigned int count, const unsigned int* data, GLenum usagePattern = GL_STATIC_DRAW);
    ElementBuffer();

    virtual void assignData(unsigned int count, const void* data, GLenum usagePattern = GL_STATIC_DRAW) override;

    virtual void bind() const override;
    static void unbindAll();

    unsigned int getCount() const;

private:
    unsigned int m_count;
};


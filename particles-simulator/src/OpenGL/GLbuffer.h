#pragma once

/**
 * Base class for OpenGL's vertex bufffers and element (index) buffers
 */
class GLbuffer
{
public:
    virtual void bind() const = 0;
    virtual void assignData(unsigned int, const void*, GLenum) = 0;
    ~GLbuffer();

protected:
    GLbuffer();
    GLbuffer(GLenum bindingTarget, unsigned int size, const void* data, GLenum usagePattern);
    void assignDataGeneral(GLenum bindingTarget, unsigned int size, const void* data, GLenum usagePattern);

    unsigned int m_id;
    bool m_isDataAssigned;
};

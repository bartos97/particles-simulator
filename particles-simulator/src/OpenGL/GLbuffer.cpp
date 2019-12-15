#include "pch.h"
#include "GLbuffer.h"

GLbuffer::~GLbuffer()
{
    GL_CALL(glDeleteBuffers(1, &m_id));
    PS_LOG("Buffer #%d deleted", m_id);
}


GLbuffer::GLbuffer()
{
    PS_LOG("Buffer created without data to bind");
    m_isDataAssigned = false;
    m_id = 0;
}


GLbuffer::GLbuffer(GLenum bindingTarget, unsigned int size, const void* data, GLenum usagePattern)
{
    m_isDataAssigned = false;
    m_id = 0;
    PS_LOG("Buffer object constructed");
    assignDataGeneral(bindingTarget, size, data, usagePattern);
}


void GLbuffer::assignDataGeneral(GLenum bindingTarget, unsigned int size, const void* data, GLenum usagePattern)
{
    GL_CALL(glGenBuffers(1, &m_id));
    GL_CALL(glBindBuffer(bindingTarget, m_id));
    GL_CALL(glBufferData(bindingTarget, size, data, usagePattern));
    m_isDataAssigned = true;
    PS_LOG("Buffer #%d data asigned.", m_id);
}

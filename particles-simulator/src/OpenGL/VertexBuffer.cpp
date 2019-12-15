#include "pch.h"
#include "VertexBuffer.h"


VertexBuffer::VertexBuffer(unsigned int size, const void* data, GLenum usagePattern)
    : GLbuffer(GL_ARRAY_BUFFER, size, data, usagePattern)
{}


void VertexBuffer::assignData(unsigned int size, const void* data, GLenum usagePattern)
{
    assignDataGeneral(GL_ARRAY_BUFFER, size, data, usagePattern);
}


void VertexBuffer::bind() const
{
    PS_ASSERT(m_isDataAssigned, "Tring to bind VertexBuffer without data assigned!");
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, m_id));
    PS_LOG("VertexBuffer #%d is now bound", m_id);
}


void VertexBuffer::unbindAll()
{
    GL_CALL(glBindBuffer(GL_ARRAY_BUFFER, 0));
    PS_LOG("No VertexBuffer bound.");
}
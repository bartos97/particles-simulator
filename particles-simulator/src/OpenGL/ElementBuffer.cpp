#include "pch.h"
#include "ElementBuffer.h"


ElementBuffer::ElementBuffer(unsigned int count, const unsigned int* data, GLenum usagePattern)
    : GLbuffer(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usagePattern), m_count(count)
{}


ElementBuffer::ElementBuffer()
    : m_count(0)
{}


void ElementBuffer::assignData(unsigned int count, const void* data, GLenum usagePattern)
{
    assignDataGeneral(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(unsigned int), data, usagePattern);
}


void ElementBuffer::bind() const
{
    PS_ASSERT(m_isDataAssigned, "Tring to bind ElementBuffer without data assigned!");
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id));
    PS_LOG("ElementBuffer #%d is now bound", m_id);
}


void ElementBuffer::unbindAll()
{
    GL_CALL(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
    PS_LOG("No ElementBuffer bound.");
}

unsigned int ElementBuffer::getCount() const
{
    return m_count;
}

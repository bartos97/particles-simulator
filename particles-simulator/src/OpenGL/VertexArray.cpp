#include "pch.h"
#include "VertexArray.h"


const VertexArray* VertexArray::m_currentlyBound = nullptr;


VertexArray::VertexArray()
    : m_vertexBuffer(nullptr), m_elementBuffer(nullptr)
{
    GL_CALL(glGenVertexArrays(1, &m_id));
    GL_CALL(glBindVertexArray(m_id));
    VertexArray::m_currentlyBound = this;
    PS_LOG("Created VertexArray #%d", m_id);
}


VertexArray::VertexArray(const VertexBuffer & vb, const ElementBuffer & eb, const BufferLayout & layout)
    : VertexArray()
{
    assignData(vb, eb, layout);
}


void VertexArray::assignData(const VertexBuffer& vb, const ElementBuffer& eb, const BufferLayout& layout)
{
    m_vertexBuffer = &vb;
    m_elementBuffer = &eb;
    addBuffer(vb, layout);
    addBuffer(eb);
}


VertexArray::~VertexArray()
{
    GL_CALL(glDeleteVertexArrays(1, &m_id));
    PS_LOG("Vertex Array #%d deleted", m_id);
}


void VertexArray::bind() const
{
    if (this != VertexArray::m_currentlyBound)
    {
        GL_CALL(glBindVertexArray(m_id));
        VertexArray::m_currentlyBound = this;
        //PS_LOG("VertexArray #%d is now bound", m_id);
    }
}


void VertexArray::unbind() const
{
    GL_CALL(glBindVertexArray(0));
    PS_LOG("No VertexArray bound.");
    VertexArray::m_currentlyBound = nullptr;
}


const VertexBuffer & VertexArray::getVertexBuffer() const
{
    PS_ASSERT(m_vertexBuffer, "There is no vertex buffer assigned to vertex array #%d", m_id);
    return *(m_vertexBuffer);
}


const ElementBuffer & VertexArray::getElementBuffer() const
{
    PS_ASSERT(m_elementBuffer, "There is no element buffer assigned to vertex array #%d", m_id);
    return *(m_elementBuffer);
}


void VertexArray::addBuffer(const VertexBuffer& vb, const BufferLayout& layout)
{
    bind();
    vb.bind();

    const auto& layoutElements = layout.getElements();
    unsigned int offset = 0;
    unsigned int stride = layout.getStride();

    for (unsigned int i = 0; i < layoutElements.size(); i++)
    {
        const auto& el = layoutElements[i];
        glVertexAttribPointer(i, el.count, el.type, el.normalized, stride, (const void*)offset);
        glEnableVertexAttribArray(i);
        offset += el.count * BufferLayout::getSizeOfGLtype(el.type);
    }

}

void VertexArray::addBuffer(const ElementBuffer& eb)
{
    bind();
    eb.bind();
}
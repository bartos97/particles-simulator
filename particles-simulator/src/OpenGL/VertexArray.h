#pragma once
#include "OpenGL/BufferLayout.h"
#include "OpenGL/VertexBuffer.h"
#include "OpenGL/ElementBuffer.h"


class VertexArray
{
public:
    ~VertexArray();

    VertexArray();
    VertexArray(const VertexBuffer& vb, const ElementBuffer& eb, const BufferLayout& layout);

    void assignData(const VertexBuffer& vb, const ElementBuffer& eb, const BufferLayout& layout);

    void bind() const;
    void unbind() const;

    const VertexBuffer& getVertexBuffer() const;
    const ElementBuffer& getElementBuffer() const;

private:
    void addBuffer(const VertexBuffer& vb, const BufferLayout& layout);
    void addBuffer(const ElementBuffer& eb);

    unsigned int m_id;
    const VertexBuffer* m_vertexBuffer;
    const ElementBuffer* m_elementBuffer;
    static const VertexArray* m_currentlyBound;
};


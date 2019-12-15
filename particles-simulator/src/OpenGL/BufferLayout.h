#pragma once


/**
 * Class for representing vertex attributes in vertex buffer
 */
class BufferLayout
{
public:
    /**
     * Nested public class that represent single element in layout:
     * that is variables in given 'type' in the amount of 'count',
     * that represent one logical elment in buffer;
     * eg. 3D vertex, that'd be 3 floats which form one BufferLayoutElement
     */
    struct BufferLayoutElement
    {
        unsigned int count;
        GLenum type;
        GLboolean normalized;

        BufferLayoutElement(unsigned int count, GLenum type, GLboolean normalized)
            : count(count), type(type), normalized(normalized)
        {}
    };

    BufferLayout()
        : m_stride(0)
    {}

    const std::vector<BufferLayout::BufferLayoutElement>& getElements() const
    {
        return m_elements;
    }

    /**
     * @param type of variable represented as GLenum (unsigned int)
     * @return size in bytes of a single variable of given type
     */
    static unsigned int getSizeOfGLtype(GLenum type);

    /**
     * @return size in bytes between one set of elements in vertex buffer and next one
     */
    unsigned int getStride() const
    {
        return m_stride;
    }

    /**
     * Add to laypout variables of type T in the amount of count,
     * that represent one logical element in vertex buffer
     */
    template<typename T>
    void add(unsigned int count)
    {
        PS_ASSERT(false, "Invalid type when adding to buffer layout!");
    }

    template<>
    void add<float>(unsigned int count)
    {
        m_elements.emplace_back(count, GL_FLOAT, GL_FALSE);
        m_stride += count * BufferLayout::getSizeOfGLtype(GL_FLOAT);
    }

    template<>
    void add<unsigned int>(unsigned int count)
    {
        m_elements.emplace_back(count, GL_UNSIGNED_INT, GL_TRUE);
        m_stride += count * BufferLayout::getSizeOfGLtype(GL_UNSIGNED_INT);
    }

    template<>
    void add<unsigned char>(unsigned int count)
    {
        m_elements.emplace_back(count, GL_UNSIGNED_BYTE, GL_TRUE);
        m_stride += count * BufferLayout::getSizeOfGLtype(GL_UNSIGNED_BYTE);
    }

private:
    std::vector<BufferLayoutElement> m_elements;
    unsigned int m_stride;
};


#include "pch.h"
#include "BufferLayout.h"


unsigned int BufferLayout::getSizeOfGLtype(GLenum type)
{
    switch (type)
    {
    case GL_FLOAT:          return sizeof(float);
    case GL_UNSIGNED_INT:   return sizeof(unsigned int);
    case GL_UNSIGNED_BYTE:  return sizeof(unsigned char);
    default:
        PS_ASSERT(false, "Invalid GL type!");
        return 0;
        break;
    }
}
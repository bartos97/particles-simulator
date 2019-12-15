#include "pch.h"

void GLerrorClear()
{
    while (glGetError() != GL_NO_ERROR);
}

bool GLerrorCheck()
{
    bool retval = true;
    while (GLenum err = glGetError())
    {
        PS_LOG("[OpenGL error]: %#06x", err);
        retval = false;
    }
    return retval;
}

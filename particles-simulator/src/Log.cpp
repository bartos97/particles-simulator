#include "pch.h"
#include "Log.h"

void Log::GLerrorClear()
{
    while (glGetError() != GL_NO_ERROR);
}

bool Log::GLerrorCheck()
{
    bool retval = true;
    while (GLenum err = glGetError())
    {
        PS_LOG("[OpenGL error]: %#06x", err);
        retval = false;
    }
    return retval;
}

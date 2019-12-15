#version 430 core

layout (location = 0) in vec2 attrPos;

uniform mat4 u_transformation;

void main()
{
    gl_Position = u_transformation * vec4(attrPos.x, attrPos.y, 0.0, 1.0);
}
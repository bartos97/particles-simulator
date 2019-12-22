#version 330 core

layout (location = 0) in vec2 a_position;

out vec4 screenPosition;

uniform mat4 u_transformation;

void main()
{
    gl_Position = screenPosition = u_transformation * vec4(a_position.xy, 0.0, 1.0);
}

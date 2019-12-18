#version 330 core

layout (location = 0) in vec2 a_position;

out vec4 interpolatedVertexPosition;

uniform mat4 u_transformation;

void main()
{
    gl_Position = interpolatedVertexPosition = u_transformation * vec4(a_position.x, a_position.y, 0.0, 1.0);
}
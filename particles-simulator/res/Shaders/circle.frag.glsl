#version 330 core

out vec4 fragmentColor;

in vec4 interpolatedVertexPosition;

uniform vec2 u_mousePosition;
uniform float u_radius;
uniform vec2 u_center;

void main()
{
    float vertDist = distance(vec2(interpolatedVertexPosition.x, interpolatedVertexPosition.y), u_center);
    if (vertDist > u_radius) discard;

    float mouseDist = distance(vec2(interpolatedVertexPosition.x, interpolatedVertexPosition.y), u_mousePosition);
    fragmentColor = vec4(mouseDist / 2.0f, 0.5, 0.6, 1.0);
}
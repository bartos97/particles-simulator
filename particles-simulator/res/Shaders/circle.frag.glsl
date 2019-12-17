#version 330 core

out vec4 fragmentColor;

in vec4 interpolatedVertexPosition;

//uniform vec2 u_mousePosition;
uniform float u_radius;
uniform vec2 u_center;

void main()
{
    float vertDist = distance(vec2(interpolatedVertexPosition.x, interpolatedVertexPosition.y), u_center);
//    float mouseDist = length(u_mousePosition);
    if (vertDist > u_radius) discard;

    fragmentColor = vec4(0.2, 0.5, 0.6, 1.0);
}
#version 330 core

out vec4 fragmentColor;

in vec4 screenPosition;

uniform vec2 u_mousePosition;
uniform float u_radius;
uniform vec2 u_center;

void main()
{
    vec2 vert = vec2(screenPosition.xy);
    vec4 staticColor = vec4(0.2f, 0.5f, 0.6f, 1.0f);

    float vertDist = distance(vert, u_center);
    if (vertDist > u_radius) discard;

    float mouseDist = (2.0f - distance(vert, u_mousePosition) * 0.8) / 2.0f;
    mouseDist = sqrt(mouseDist);
    fragmentColor = staticColor * mouseDist;
}
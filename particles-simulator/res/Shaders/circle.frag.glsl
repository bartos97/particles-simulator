#version 330 core

out vec4 fragmentColor;

in vec4 screenPosition;

uniform vec2 u_mousePosition;
uniform float u_radius;
uniform vec2 u_center;
uniform vec4 u_color;

const float twoSqrt2 = 2 * 1.41421f;

void main()
{
    vec2 vert = vec2(screenPosition.xy);
    float pixelDist = distance(vert, u_center);
    if (pixelDist > u_radius) discard;

    float mouseDist = (twoSqrt2 - distance(vert, u_mousePosition) * 0.8) / twoSqrt2;
    mouseDist = sqrt(mouseDist);

    float centerDist = (u_radius - pixelDist * 0.55) / u_radius;
    centerDist = clamp(centerDist * centerDist, 0.0f, 1.0f);

    float centerDist2 = (u_radius - pixelDist * 0.75) / u_radius;
    centerDist2 = clamp(sqrt(centerDist2), 0.0f, 1.0f);

    vec4 toWhiteComplement = vec4(0.8f, 0.8f, 0.8f, 1.0f) - u_color;

    fragmentColor = (u_color + toWhiteComplement * centerDist) * centerDist2 * mouseDist;
//    fragmentColor = u_color;
}
#version 450

#include "utils.glsl"

layout(location = 0) out vec4 out_color;

layout(location = 0) in vec2 in_uv;

layout(binding = 0) uniform sampler2D in_tex;

uniform uint selectedTexture = 0;

void main()
{
    // const vec3 color = in_color;
    // TODO
    if (selectedTexture == 0)
    {
        out_color = vec4(1.0, 0.0, 0.0, 1.0);
    }
    else if (selectedTexture == 1)
    {
        out_color = vec4(0.0, 1.0, 0.0, 1.0);
    }
    else if (selectedTexture == 2)
    {
        out_color = vec4(0.0, 0.0, 1.0, 1.0);
    }
    else
    {
        // Unknown
        out_color = vec4(1.0, 0.0, 1.0, 1.0);
    }
}

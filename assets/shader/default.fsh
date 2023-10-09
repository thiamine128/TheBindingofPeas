#version 330 core

layout (location = 0) out vec3 gNormal;
layout (location = 1) out vec3 gColor;

in vec2 TexCoord;
in vec4 PosLightSpace;
in vec3 Normal;
in vec3 Pos;



uniform sampler2D texture0;
uniform sampler2D shadowMap;
uniform vec3 lightPos;
uniform vec4 color;

vec2 poissonDisk[4] = vec2[](
  vec2(-0.94201624, -0.39906216),
  vec2(0.94558609, -0.76890725),
  vec2(-0.094184101, -0.92938870),
  vec2(0.34495938, 0.29387760)
);

float calcShadow(vec4 pos) {
    vec3 coord = pos.xyz / pos.w;
    coord = coord * 0.5 + 0.5;

    if (coord.x > 1.0 || coord.y > 1.0 || coord.x < 0.0 || coord.y < 0.0)
        return 1.0;
    float shadow = 1.0;
    vec3 lightDir = normalize(lightPos);
    vec3 normal = normalize(Normal);
    float bias = max(0.01 * (0.5 - dot(normal, lightDir)), 0.006);
    float dense = 0.04f;
    for (int i = 0; i < 4; ++i) {
        if (texture(shadowMap, coord.xy + poissonDisk[i] / 700.0).r < coord.z - bias) {
            shadow -= dense;
        }
    }
    return shadow;
}

void main()
{
    gNormal = normalize(Normal);
    gColor = color.rgb;
    gColor *= texture(texture0, TexCoord).rgb * calcShadow(PosLightSpace);
}
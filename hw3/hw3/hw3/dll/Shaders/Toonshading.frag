#version 430

uniform sampler2D texture;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform vec3 La;
uniform vec3 Ld;
uniform vec3 Ls;
uniform int gloss;
uniform vec3 WorldLightPos;
uniform vec3 WorldCamPos;
uniform float cutoff;
uniform int edgefalloff;
in vec2 uv;
in vec3 normal;
in vec4 worldPos;

out vec4 color;
void main()
{
    color = texture2D(texture, uv);
    vec3 L = normalize(WorldLightPos - worldPos.xyz);
    float level = dot(normal, L);
    float intensity;
    if(level > 0.95)intensity = 1;
    else if(level > 0.75)intensity = 0.8;
    else if(level > 0.5)intensity = 0.6;
    else if(level > 0.25)intensity = 0.4;
    else intensity = 0.2;
    color = vec4(Kd * color.xyz * intensity, 1.0);
} 
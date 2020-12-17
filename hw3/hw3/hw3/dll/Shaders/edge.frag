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
    vec3 v = normalize(WorldCamPos - worldPos.xyz);
    float intensity;
    intensity = dot(v, normal);
    intensity = abs(intensity);
    intensity = 1.0 - pow(intensity, edgefalloff);
    if(intensity > cutoff)intensity = 1;
    else intensity = 0;
    vec3 color_edge =  intensity * vec3(0,1,1);
    color = vec4(color_edge, 1.0);
} 
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
    float diff  = max(dot(L, normal), 0.0);
    vec3 r = normalize(reflect(-L, normal));
    vec3 v = normalize(WorldCamPos - worldPos.xyz);
    float spec = max(dot(v, r), 0.0);
    vec3 ambient = La * Ka * color.xyz;
    vec3 diffuse = Ld * Kd * color.xyz * diff;
    vec3 specular = Ls * Ks * pow(spec, gloss);
    vec3 color_phong = ambient + diffuse + specular;
    color = vec4(color_phong, 1.0f);
} 
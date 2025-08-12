// fragment.glsl
#version 330 core

#define MAX_UBO_LIGHTS 16

in vec2 vUV;
in vec3 vNormal;
in vec3 vPos;

struct Light
{
	unsigned int type;
	float colorIntensity;
	vec2 spotlight;
	vec4 color;
	vec4 lightDir;
	vec4 attenuation; 
};

// Uniform object for lights
layout(std140, binding = 0) uniform LightsUBO
{
    ivec lightCount;
    Light lights[MAX_UBO_LIGHTS];
}

uniform sampler2D aTexture;
out vec4 FragColor;

void main()
{
    FragColor = texture(aTexture, vUV);
}
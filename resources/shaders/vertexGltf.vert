// vertex.glsl
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;
layout(location = 2) in vec2 aUV;

uniform mat4  u_MVP;

out vec2 vUV;
out vec3 vNormal;
out vec3 vPos;

void main()
{
    vNormal     = normal;
    vUV         = aUV;
    vPos        = aPos;
    gl_Position = u_MVP * vec4(aPos, 1.0);
}
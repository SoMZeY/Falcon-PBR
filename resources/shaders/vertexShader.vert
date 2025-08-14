// vertex.glsl
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;

uniform mat4 u_MVP;

out vec3 vNormal;
out vec3 vPos;

void main()
{
    vNormal     = normal;
    vPos        = aPos;
    gl_Position = u_MVP * vec4(aPos, 1.0);
}
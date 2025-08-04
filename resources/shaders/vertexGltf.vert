// vertex.glsl
#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec2 aUV;

uniform mat4 u_MVP;
out vec2 vUV;

void main()
{
    vUV         = aUV;
    gl_Position = u_MVP * vec4(aPos, 1.0);
}
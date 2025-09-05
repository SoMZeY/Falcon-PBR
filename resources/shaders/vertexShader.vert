// vertexShader.glsl
#version 420 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 normal;

uniform mat4 u_MVP;        // Projection * View * Model
uniform mat4 u_ModelView;  // View * Model

// View-space varyings
out vec3 vNormal;
out vec3 vPos;

void main()
{
    // View-space position
    vec4 posVS4 = u_ModelView * vec4(aPos, 1.0);
    vPos = posVS4.xyz;

    // View-space normal
    mat3 normalMat = transpose(inverse(mat3(u_ModelView)));
    vNormal = normalize(normalMat * normal);

    // Clip-space position
    gl_Position = u_MVP * vec4(aPos, 1.0);
}

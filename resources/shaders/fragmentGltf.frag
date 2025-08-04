// fragment.glsl
#version 330 core
in  vec2 vUV;

uniform sampler2D aTexture;   // bound to texture unit 0
out vec4 FragColor;

void main()
{
    FragColor = texture(aTexture, vUV);
}
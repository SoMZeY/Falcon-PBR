#version 420

layout(location = 0) in vec3 aPos;

out vec3 TexCoords;

uniform mat4 u_MVP;
uniform mat4 u_ModelView;

void main()
{
	TexCoords = aPos;
	vec4 clippedPos = u_MVP * vec4(aPos, 1.0);
	gl_Position = clippedPos.xyww;
}
#version 330 core
								 
layout (location = 0) in vec3 a_Pos;
layout (location = 1) in vec3 aColor;
//layout (location = 2) in vec2 aTexCoord;

out vec3 ourColor;
//out vec2 texCoord;

uniform mat4 u_View;
uniform mat4 u_Projection;
uniform mat4 u_Model;

void main()
{
	gl_Position = u_Projection * u_View * u_Model * vec4(a_Pos, 1.0);

	//gl_Position = vec4(a_Pos, 1.0)
	ourColor = aColor;
	//texCoord = aTexCoord;
}
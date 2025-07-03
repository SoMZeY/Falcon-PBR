#version 330 core

out vec4 FragColor;
in vec3 ourColor;
//in vec2 texCoord;

//uniform vec2 u_resolution;
//uniform float u_time;

//uniform sampler2D ourTexture;
//uniform sampler2D ourTexture1;

void main()
{
    //vec2 st = gl_FragCoord.xy / u_resolution;
	//FragColor = vec4(abs(sin(1-u_time * 0.2f)), abs(sin(u_time * 0.2)), 0.0f, 1.0f);
	//FragColor = mix(texture(ourTexture1, texCoord), texture(ourTexture, texCoord), 0.3);
	FragColor = vec4(ourColor, 1.0f);
}
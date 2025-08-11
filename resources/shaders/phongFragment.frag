// fragment.glsl
#version 330 core

in vec2 vUV;
in vec3 vNormal;
in vec3 vPos;

uniform mat4 u_Model;
uniform sampler2D aTexture;
out vec4 FragColor;

uniform vec3  u_lightLocation;
uniform vec3  u_lightColor;
uniform float u_lightIntensity;

uniform vec3  u_cameraPos;

void main()
{
    vec3 worldNormal = normalize(mat3(transpose(inverse(u_Model))) * vNormal);
    vec3 worldPos    = vec3(u_Model * vec4(vPos, 1.0f));
    vec3 lightDir    = normalize(u_lightLocation - worldPos);

    // Ambient
    float ambient = 0.1f;

    // Diffuse
    vec3 diffuse = u_lightIntensity * u_lightColor * max(dot(worldNormal, lightDir), 0.0f);

    // Specular
    float specularStrength = 0.9f;
    vec3 viewDir = normalize(u_cameraPos - worldPos);
    vec3 reflectDir = reflect(-lightDir, worldNormal);
    vec3 specular = specularStrength * pow(max(dot(viewDir, reflectDir), 0.0f), 64) * u_lightColor;

    vec3 phongLight = ambient + diffuse + specular;

    FragColor = vec4(phongLight, 1.0f) * texture(aTexture, vUV);
}
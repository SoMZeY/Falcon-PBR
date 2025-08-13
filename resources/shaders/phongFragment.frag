#version 420 core
#define MAX_UBO_LIGHTS 16

in vec2 vUV;
in vec3 vNormal;
in vec3 vPos;

uniform vec3 u_ViewDir;
uniform vec3 u_CameraPos;
uniform sampler2D aTexture;

struct Light {
    uint  type;            // 0 = directional, 1 = point, 2 = spotlight
    float colorIntensity;
    vec2  spotlight;       // x innerCone, y outerCone
    vec4  color;           // rgb + unused a
    vec4  lightDir;        // dir for dir light; *position* for point (xyz)
    vec4  attenuation;     // (k0, k1, k2, unused)
};

layout(std140, binding = 0) uniform LightsUBO {
    ivec4 lightCount;
    Light lights[MAX_UBO_LIGHTS];
} u_Lights;

out vec4 FragColor;

vec3 CalcDirLight(in Light light, in vec3 normal, in vec3 viewDir);
vec3 CalcPointLight(in Light light, in vec3 normal, in vec3 viewDir, in vec3 fragPos);
vec3 CalcSpotLight(in Light light, in vec3 normal, in vec3 viewDir, in vec3 fragPos);

void main()
{
    vec3 albedo  = texture(aTexture, vUV).rgb;
    vec3 normal  = normalize(vNormal);
    vec3 viewDir = normalize(u_CameraPos - vPos);
    vec3 spotDir = normalize(u_ViewDir);

    vec3 lighting = vec3(0.0);

    int count = min(u_Lights.lightCount.x, MAX_UBO_LIGHTS);
    for (int i = 0; i < count; ++i)
    {
        Light L = u_Lights.lights[i];
        if (L.type == uint(0)) 
        {
            lighting += CalcDirLight(L, normal, viewDir);
        } 
        else if (L.type == uint(1)) 
        {
            lighting += CalcPointLight(L, normal, viewDir, vPos);;
        }
        else if (L.type == uint(2)) 
        {
            lighting += CalcSpotLight(L, normal, spotDir, vPos);
        }
    }

    FragColor = vec4(lighting * albedo, 1.0);
}

vec3 CalcDirLight(in Light light, in vec3 normal, in vec3 viewDir)
{
    vec3 L = normalize(light.lightDir.xyz);
    float diff = max(dot(normal, L), 0.0);

    vec3 R = reflect(-L, normal);
    float spec = pow(max(dot(viewDir, R), 0.0), 64);

    vec3 ambient  = light.color.rgb * light.colorIntensity;
    vec3 diffuse  = light.color.rgb * diff;
    vec3 specular = light.color.rgb * spec;

    return ambient + diffuse + specular;
}

vec3 CalcPointLight(in Light light, in vec3 normal, in vec3 viewDir, in vec3 fragPos)
{
    // Interpret light.lightDir.xyz as *position* for point lights
    vec3 toLight = light.lightDir.xyz - fragPos;
    float dist   = length(toLight);
    vec3  L      = dist > 0.0 ? toLight / dist : vec3(0.0);

    // Lambert + Phong
    float diff = max(dot(normal, L), 0.0);

    // Attenuation: attenuation.xyz = (k0, k1, k2)
    float k0 = light.attenuation.x;
    float k1 = light.attenuation.y;
    float k2 = light.attenuation.z;
    float att = 1.0 / max(k0 + k1 * dist + k2 * dist * dist, 1e-4);

    // Simple specular
    vec3 R = reflect(-L, normal);
    float spec = pow(max(dot(viewDir, R), 0.0), 32.0);

    vec3 diffuse  = light.color.rgb * diff;
    vec3 specular = light.color.rgb * spec;

    return (diffuse + specular) * att * light.colorIntensity;
}

vec3 CalcSpotLight(in Light light, in vec3 normal, in vec3 spotDir, in vec3 fragPos)
{
    vec3 toLight = light.lightDir.xyz - fragPos;
    float dist   = length(toLight);
    vec3  L      = dist > 0.0 ? toLight / dist : vec3(0.0);

    // Cone test: compare spotlight forward with vector from light to fragment (-L)
    float theta  = dot(normalize(spotDir), -L);

    vec3 finalColor = vec3(0.0);
    if (theta > light.spotlight.x) 
    {
        finalColor = light.color.rgb;
    } 
    else if (theta > light.spotlight.y) 
    {
        float edge = clamp((theta - light.spotlight.y) /
                           max(light.spotlight.x - light.spotlight.y, 1e-4), 0.0, 1.0);
        finalColor = light.color.rgb * edge;
    }

    // Diffuse
    float diff = max(dot(normalize(normal), L), 0.0);

    // Specular calculation
    vec3 R = reflect(-L, normalize(normal));
    vec3 V = normalize(u_CameraPos - fragPos);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    // (Optional but recommended) distance attenuation like point lights
    float k0 = light.attenuation.x, k1 = light.attenuation.y, k2 = light.attenuation.z;
    float att = 1.0 / max(k0 + k1*dist + k2*dist*dist, 1e-4);

    vec3 diffuse  = finalColor * diff;
    vec3 specular = finalColor * spec;

    return (diffuse + specular) * light.colorIntensity * att; // add *att if you enabled it
}
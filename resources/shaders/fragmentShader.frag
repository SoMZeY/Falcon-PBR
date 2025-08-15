// fragmentFloor.glsl
#version 420 core

in vec3 vNormal;  // view-space
in vec3 vPos;     // view-space
out vec4 FragColor;

// ---- Lights UBO (std140) ----
#define MAX_LIGHTS 16
struct Light {
    vec4  color;       // rgb
    vec4  position;    // view-space (unused for dir)
    vec4  lightDir;    // normalized view-space, w=0
    vec4  attenuation; // x=c, y=l, z=q
    vec2  spotlight;   // cos(inner), cos(outer)
    float intensity;
    int   type;        // 0=dir, 1=point, 2=spot
};

layout(std140, binding = 0) uniform LightsUBO {
    uvec4 header;
    Light lights[MAX_LIGHTS];
};

vec3 evalLight(in Light L, in vec3 fragPosVS, in vec3 N)
{
    vec3 Lvec;
    float att = 1.0;

    if (L.type == 0) {
        // Directional: -lightDir points from light to fragment
        Lvec = normalize(-L.lightDir.xyz);
    } else {
        vec3 toLight = L.position.xyz - fragPosVS;
        float dist = length(toLight);
        Lvec = toLight / max(dist, 1e-6);
        att = 1.0 / (L.attenuation.x + L.attenuation.y * dist + L.attenuation.z * dist * dist);
    }

    // Spot cone
    float spot = 1.0;
    if (L.type == 2) {
        float cosTheta = dot(-L.lightDir.xyz, Lvec);
        float inner = L.spotlight.x, outer = L.spotlight.y;
        float t = clamp((cosTheta - outer) / max(inner - outer, 1e-5), 0.0, 1.0);
        spot = t * t * (3.0 - 2.0 * t);
    }

    float NdotL = max(dot(N, Lvec), 0.0);

    // Classic Phong specular
    vec3 V = normalize(-fragPosVS);   // camera at origin in view space
    vec3 R = reflect(-Lvec, N);
    float spec = pow(max(dot(V, R), 0.0), 64.0) * 0.9;

    vec3 radiance = L.color.rgb * L.intensity * att * spot;
    return radiance * (NdotL + spec);
}

void main()
{
    // Solid floor color; change as needed
    vec3 albedo = vec3(0.8);

    vec3 N = normalize(vNormal);
    vec3 ambient = 0.05 * albedo;

    uint count = min(header.x, uint(MAX_LIGHTS));
    vec3 sumLights = vec3(0.0);
    for (uint i = 0u; i < count; ++i)
        sumLights += evalLight(lights[i], vPos, N) * albedo;

    FragColor = vec4(ambient + sumLights, 1.0);
}

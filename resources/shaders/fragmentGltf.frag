// fragmentGltf.glsl
#version 420 core

in vec2 vUV;

// These are in VIEW SPACE
in vec3 vNormal;
in vec3 vPos;

out vec4 FragColor;

uniform sampler2D aTexture;

// ---- Lights UBO (std140) ----
#define MAX_LIGHTS 16

struct Light {
    vec4  color;       // rgb color, a unused
    vec4  position;    // view-space; for dir lights unused/zero
    vec4  lightDir;    // normalized view-space dir (w=0)
    vec4  attenuation; // x=c, y=l, z=q, w unused
    vec2  spotlight;   // cos(inner), cos(outer)
    float intensity;   // scalar multiplier
    int   type;        // 0=dir, 1=point, 2=spot
};

layout(std140, binding = 0) uniform LightsUBO {
    uvec4 header;                // header.x == light count
    Light lights[MAX_LIGHTS];
};

// ---- Phong evaluation in view space ----
vec3 evalLight(in Light L, in vec3 fragPosVS, in vec3 N)
{
    // Direction from fragment toward light
    vec3 Lvec;
    float attenuation = 1.0;

    if (L.type == 0) {
        // Directional: lightDir points FROM light TO scene
        Lvec = normalize(-L.lightDir.xyz);
    } else {
        // Point/Spot: position is in view space
        vec3 toLight = L.position.xyz - fragPosVS;
        float dist = length(toLight);
        Lvec = toLight / max(dist, 1e-6);
        attenuation = 1.0 / (L.attenuation.x + L.attenuation.y * dist + L.attenuation.z * dist * dist);
    }

    // Spotlight cone
    float spot = 1.0;
    if (L.type == 2) {
        // Compare angle between -lightDir and Lvec
        float cosTheta = dot(L.lightDir.xyz, -Lvec);
        float inner = L.spotlight.x;
        float outer = L.spotlight.y;
        // Smooth falloff between outer..inner
        float t = clamp((cosTheta - outer) / max(inner - outer, 1e-5), 0.0, 1.0);
         // Smoothstep function
        spot = t * t * (3.0 - 2.0 * t);
    }

    // Diffuse
    float NdotL = max(dot(N, Lvec), 0.0);

    // Specular (Here classic Phong with reflect)
    vec3 V = normalize(-fragPosVS);
    vec3 R = reflect(-Lvec, N);
    float specPower = 64.0;
    float specularStrength = 0.9;
    float spec = pow(max(dot(V, R), 0.0), specPower) * specularStrength;

    // Final light contribution
    vec3 radiance = L.color.rgb * L.intensity * attenuation * spot;
    return radiance * (NdotL + spec);
}

void main()
{
    // Base albedo from texture
    vec3 albedo = texture(aTexture, vUV).rgb;

    // Normalize view-space normal
    vec3 N = normalize(vNormal);

    // Ambient term
    vec3 ambient = 0.1 * albedo;

    // Accumulate all lights from the UBO
    uint count = header.x;
    count = min(count, uint(MAX_LIGHTS));

    vec3 sumLights = vec3(0.0);
    for (uint i = 0u; i < count; ++i) {
        sumLights += evalLight(lights[i], vPos, N) * albedo;
    }

    FragColor = vec4(ambient + sumLights, 1.0);
}

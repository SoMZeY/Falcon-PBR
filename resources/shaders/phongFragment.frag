#version 420 core
#define MAX_UBO_LIGHTS 16

// View-space from the vertex shader
in vec2 vUV;
in vec3 vNormalVS;   // must be view-space
in vec3 vPosVS;      // must be view-space

uniform sampler2D aTexture;

// ---- UBO layout must match C++ LightWS EXACTLY (std140) ----
struct LightVS {
    vec4 color;        // rgb + unused a
    vec4 position;     // view-space position; w=1 for point/spot; unused for dir
    vec4 dir;          // normalized view-space direction; w=0
    vec4 attenuation;  // (k0, k1, k2, unused)
    vec2 spotlight;    // (innerCos, outerCos)
    float intensity;   // scalar multiplier
    int   type;        // 0=dir, 1=point, 2=spot
};

layout(std140, binding = 0) uniform LightsUBO {
    uvec4 lightCount;                 // x = number of active lights
    LightVS lights[MAX_UBO_LIGHTS];
} u_Lights;

// Output
out vec4 FragColor;

// Prototypes
vec3 CalcDirLight (in LightVS L, in vec3 N, in vec3 V);
vec3 CalcPointLight(in LightVS L, in vec3 N, in vec3 V, in vec3 P);
vec3 CalcSpotLight (in LightVS L, in vec3 N, in vec3 V, in vec3 P);

void main()
{
    // View-space basis
    vec3 albedo = texture(aTexture, vUV).rgb;
    vec3 N  = normalize(vNormalVS);
    vec3 P  = vPosVS;
    vec3 V  = normalize(-P);   // camera is at origin in view space

    vec3 lighting = vec3(0.0);

    int count = int(min(u_Lights.lightCount.x, uint(MAX_UBO_LIGHTS)));
    for (int i = 0; i < count; ++i)
    {
        LightVS L = u_Lights.lights[i];

        if (L.type == 0) {
            lighting += CalcDirLight(L, N, V);
        } else if (L.type == 1) {
            lighting += CalcPointLight(L, N, V, P);
        } else { // L.type == 2
            lighting += CalcSpotLight(L, N, V, P);
        }
    }

    FragColor = vec4(lighting * albedo, 1.0);
}

// ---- Lighting in view space ----

vec3 CalcDirLight(in LightVS L, in vec3 N, in vec3 V)
{
    // L.dir points FROM light TO scene; vector to light = -L.dir
    vec3 Lvec = normalize(-L.dir.xyz);

    float diff = max(dot(N, Lvec), 0.0);

    vec3 R = reflect(-Lvec, N);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    vec3 radiance = L.color.rgb * L.intensity;
    // Optional ambient per-light (commented to avoid double ambient):
    // vec3 ambient = 0.0 * radiance;

    return radiance * (diff + spec);
}

vec3 CalcPointLight(in LightVS L, in vec3 N, in vec3 V, in vec3 P)
{
    vec3 toLight = L.position.xyz - P;
    float dist   = length(toLight);
    vec3  Lvec   = (dist > 0.0) ? toLight / dist : vec3(0.0);

    float diff = max(dot(N, Lvec), 0.0);

    float k0 = L.attenuation.x;
    float k1 = L.attenuation.y;
    float k2 = L.attenuation.z;
    float att = 1.0 / max(k0 + k1 * dist + k2 * dist * dist, 1e-4);

    vec3 R = reflect(-Lvec, N);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    vec3 radiance = L.color.rgb * L.intensity * att;
    return radiance * (diff + spec);
}

vec3 CalcSpotLight(in LightVS L, in vec3 N, in vec3 V, in vec3 P)
{
    vec3 toLight = L.position.xyz - P;
    float dist   = length(toLight);
    vec3  Lvec   = (dist > 0.0) ? toLight / dist : vec3(0.0);

    // Spotlight cone: compare forward (L.dir) with vector toward fragment (-Lvec)
    float cosTheta = dot(normalize(L.dir.xyz), -Lvec);

    // Smooth edge between outer and inner cones
    float inner = L.spotlight.x;
    float outer = L.spotlight.y;
    float t = clamp((cosTheta - outer) / max(inner - outer, 1e-4), 0.0, 1.0);
    float cone = t * t * (3.0 - 2.0 * t); // smoothstep

    // Distance attenuation (same as point)
    float k0 = L.attenuation.x;
    float k1 = L.attenuation.y;
    float k2 = L.attenuation.z;
    float att = 1.0 / max(k0 + k1 * dist + k2 * dist * dist, 1e-4);

    float diff = max(dot(N, Lvec), 0.0);

    vec3 R = reflect(-Lvec, N);
    float spec = pow(max(dot(V, R), 0.0), 32.0);

    vec3 radiance = L.color.rgb * L.intensity * att * cone;
    return radiance * (diff + spec);
}

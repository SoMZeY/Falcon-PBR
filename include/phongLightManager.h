#ifndef PHONG_LIGHT_MANAGER_H
#define PHONG_LIGHT_MANAGER_H

#include <glm/glm.hpp>
#include <vector>

#include "transform.h"

enum class LightcasterType
{
	TYPE_UNKNOWN = -1,
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	SPOT_LIGHT,
};

// Struct for desribing the light
struct LightDesc {
	LightcasterType type;
	Transform       transform;
	glm::vec3       color;
	float           intensity;
	glm::vec3       attenuation;
	glm::vec2       spotAnglesCos;
};

struct alignas(16) LightWS {
	glm::vec4 color;        // rgb = light color, a unused
	glm::vec4 positionWS;   // xyz = world-space position, w=1 for point/spot; 
							// for directional lights you can leave as (0,0,0,0) or unused
	glm::vec4 dirWS;        // normalized world-space direction, w=0
	glm::vec4 attenuation;  // x = constant, y = linear, z = quadratic, w unused
	glm::vec2 spotlight;    // cos(inner), cos(outer)
	float     intensity;    // brightness multiplier
	int       type;         // 0=dir, 1=point, 2=spot
};

static_assert(sizeof(LightWS) == 80, "std140 mismatch with GLSL Light");

class PhongLightingManager
{
public:
	PhongLightingManager() : m_Count(0), m_DirectionalLightCount(0), m_PointLightCount(0), m_SpotLightCount(0) {};
	int AddLight(const LightDesc& lv);
	bool getUboPhongLights(std::vector<LightWS>& uboLightsCollection);
	void EditLight(int lightId, const LightDesc& lv);

private:
	LightWS BuildUboStruct(const LightDesc& cpuLight);

private:
	size_t m_Count;
	size_t m_DirectionalLightCount;
	size_t m_PointLightCount;
	size_t m_SpotLightCount;

	// Cache for GPU formatted light struct in world coordinates
	std::vector<LightWS> m_UboLightsWorld;

	// Cache for CPU descripted lights
	std::vector<LightDesc> m_Lights;
};

#endif
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

// std140 compliant struct that will be passed into fragment shader
struct alignas(16) LightValues {
	glm::vec4		color;         // rgb, a unused
	glm::vec4		position;      // xyz, w=1 for point/spot; undefined for directional
	glm::vec4		lightDir;      // normalized dir in chosen space, w=0
	glm::vec4		attenuation;   // x=c, y=l, z=q, w=unused
	glm::vec2		spotlight;     // innerCos, outerCos
	float			intensity;     // separate from color
	int				type;          // 0=dir,1=point,2=spot (std140: int)
};

static_assert(sizeof(LightValues) == 80, "std140 mismatch with GLSL Light");

class PhongLightingManager
{
public:
	PhongLightingManager() : m_Count(0), m_DirectionalLightCount(0), m_PointLightCount(0), m_SpotLightCount(0) {};
	unsigned int AddLight(const LightValues& lv);
	std::vector<LightValues>& getAllLightValues();
	
private:
	size_t m_Count;
	size_t m_DirectionalLightCount;
	size_t m_PointLightCount;
	size_t m_SpotLightCount;
	std::vector<LightDesc> m_Lights;
	std::vector<Transform>	 m_LightTransform;
};

#endif
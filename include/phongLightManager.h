#ifndef PHONG_LIGHT_MANAGER_H
#define PHONG_LIGHT_MANAGER_H

#include <glm/glm.hpp>
#include <vector>

enum class LightcasterType
{
	TYPE_UNKNOWN = -1,
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	FLASH_LIGHT,
};

struct alignas(16) LightValues
{
	LightcasterType type;
	float colorIntensity;
	glm::vec2 spotlight;

	// Phong Light values, xyz = color
	glm::vec4 color;

	// Can represent position based on homogeneous values
	glm::vec4 lightDir;

	// Attenuation specific: x = constant, y = linear, z = quadratic, w = nothing
	glm::vec4 attenuation; 
};
static_assert(sizeof(LightValues) == 64, "std140 mismatch with GLSL Light");

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
	std::vector<LightValues> m_LightValues;
};

#endif
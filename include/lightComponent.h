#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

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

class PhongLightComponent
{
public:
	PhongLightComponent(LightcasterType casterType, const glm::vec4& lightColor, float lightIntensity,
		const glm::vec4& worldPosition, const glm::vec3& attenuationVals, const glm::vec2& spotLightValues);

	PhongLightComponent(LightValues lightVals);

	LightValues getLightValues() const;
	

private:
	glm::vec4 convertDegreesToVec4(const glm::vec2& flashlightVals);

private:
	LightValues lightValues;
};

#endif
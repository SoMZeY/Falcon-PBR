#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include <glm/glm.hpp>
#include <vector>

enum LightcasterType
{
	TYPE_UNKNOWN = -1,
	DIRECTIONAL_LIGHT,
	POINT_LIGHT,
	FLASH_LIGHT,
};

struct alignas(16) LightValues
{
	// Type of caster
	LightcasterType type;

	// Phong Light values, xyz = color, w = type
	glm::vec3 color;

	// General light
	// Can represent position based on homogeneous values
	glm::vec4 lightDir;

	// Intensity of the light color
	float intensity;

	// Attenuation specific: x = constant, y = linear, z = quadratic
	glm::vec3 attenuation; 

	// Flashlight specific; x = innerConeRadius, y = outerConeRadius, z/w = nothing
	glm::vec4 flashlight; 
};

class PhongLightComponent
{
public:
	PhongLightComponent(LightcasterType casterType, const glm::vec3& lightColor, float lightIntensity,
		const glm::vec3& worldPosition, const glm::vec3& attenuationVals, const glm::vec2& flashLightValues);

	LightValues getLightValues() const;

private:
	glm::vec4 convertDegreesToVec4(const glm::vec2& flashlightVals);

private:
	LightValues lightValues;
};

#endif
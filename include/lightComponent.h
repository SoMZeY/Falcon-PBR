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
	// General light
	glm::vec4 lightDir; // Can represent position based on homogeneous values

	// Phong Light values
	glm::vec4 color;

	// Attenuation specific: x = constant, y = linear, z = quadratic, w = nothing
	glm::vec4 attenuation; 

	// Flashlight specific; x = innerConeRadius, y = outerConeRadius, z/w = nothing
	glm::vec4 flashlight; 
};

class PhongLightComponent
{
public:
	PhongLightComponent(const glm::vec3& lightColor, float lightIntensity, const glm::vec3& worldPosition) 
		: color(lightColor), intensity(lightIntensity), position(worldPosition) {};

	glm::vec3 GetColor();
	float GetIntensity();
	glm::vec3 GetWorldPosition();
private:
	glm::vec3 color;
	float intensity;
	LightValues lightValues;

	// TODO; refactor later with the transform object
	glm::vec3 position;
};

#endif
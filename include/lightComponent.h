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

struct LightValues
{
	// General light
	glm::vec3 lightDir; // Can represent position based on homogeneous values

	// Phong Light values
	glm::vec3 ambient;
	glm::vec3 diffuse;
	glm::vec3 specular;

	// Attenuation specific
	float constant;
	float linear;
	float quadratic;

	// Flashlight specific
	float innerCone;
	float outerCone;
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
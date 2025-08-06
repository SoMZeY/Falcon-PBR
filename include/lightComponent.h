#ifndef LIGHT_COMPONENT_H
#define LIGHT_COMPONENT_H

#include <glm/glm.hpp>

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

	// TODO; refactor later with the transform object
	glm::vec3 position;
};

#endif
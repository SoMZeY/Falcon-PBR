#include "lightComponent.h"

glm::vec3 PhongLightComponent::GetColor()
{
	return color;
}

float PhongLightComponent::GetIntensity()
{
	return intensity;
}

glm::vec3 PhongLightComponent::GetWorldPosition()
{
	return position;
}

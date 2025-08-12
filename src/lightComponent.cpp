#include "lightComponent.h"

PhongLightComponent::PhongLightComponent(LightcasterType casterType, const glm::vec3& lightColor, float lightIntensity,
	const glm::vec3& worldPosition, const glm::vec3& attenuationVals, const glm::vec2& flashLightValues)
{
	LightValues light;
	light.type = casterType;
	light.color = lightColor;
	light.intensity = lightIntensity;
	light.attenuation = attenuationVals;
	light.flashlight = convertDegreesToVec4(flashLightValues);

	lightValues = light;
}

LightValues PhongLightComponent::getLightValues() const
{
	return lightValues;
}

glm::vec4 PhongLightComponent::convertDegreesToVec4(const glm::vec2& flashlightVals)
{
	float innerCone = glm::cos(glm::radians(flashlightVals.x));
	float outerCone = glm::cos(glm::radians(flashlightVals.y));

	return glm::vec4(innerCone, outerCone, 0, 0);
}

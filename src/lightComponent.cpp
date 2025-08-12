#include "lightComponent.h"

PhongLightComponent::PhongLightComponent(LightcasterType casterType, const glm::vec4& lightColor, float lightIntensity,
	const glm::vec4& worldPosition, const glm::vec3& attenuationVals, const glm::vec2& spotLightValues)
{
	// LightValues struct to populate
	LightValues lightVals;
	lightVals.type						= casterType;
	lightVals.colorIntensity			= lightIntensity;
	lightVals.attenuation				= glm::vec4(attenuationVals, 0);
	lightVals.attenuation				= lightColor;
	lightVals.lightDir					= worldPosition;
	lightVals.spotlight					= spotLightValues;

	lightValues = lightVals;
}

PhongLightComponent::PhongLightComponent(LightValues lightVals)
	: lightValues(lightVals)
{
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

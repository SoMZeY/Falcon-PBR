#include "phongLightManager.h"
#include <iostream>

int PhongLightingManager::AddLight(const LightDesc& lv)
{
	// Keep track of the different casters type
	if (lv.type == LightcasterType::DIRECTIONAL_LIGHT) m_DirectionalLightCount++;
	else if (lv.type == LightcasterType::POINT_LIGHT)  m_PointLightCount++;
	else if (lv.type == LightcasterType::SPOT_LIGHT)   m_SpotLightCount++;
	else
	{
		std::cout << "Error: Light caster type is unknown" << std::endl;
		return -1;
	}

	m_Lights.push_back(lv);
	m_Count++;
	return m_Count - 1;
}

bool PhongLightingManager::getUboPhongLights(std::vector<LightWS>& uboLightsCollection)
{	
	// Clear the existing std140 compliant struct collection, so it would be updated
	m_UboLightsWorld.clear();

	// Loop each CPU light description, and convert to std140 compliant struct
	for (const LightDesc& cl : m_Lights)
	{
		// Pass each struct to helper function and append to the vector
		m_UboLightsWorld.push_back(BuildUboStruct(cl));
	}

	// After conversion done, cache it and return true
	uboLightsCollection = m_UboLightsWorld;
	return true;
}

void PhongLightingManager::EditLight(int lightId, const LightDesc& lv)
{
	m_Lights[lightId] = lv;
}

LightWS PhongLightingManager::BuildUboStruct(const LightDesc& cpuLight)
{	
	LightWS std140Light;
	std140Light.positionWS = cpuLight.transform.GetWorldLocation();
	// If direction light populate position with vec(0)
	if (cpuLight.type == LightcasterType::DIRECTIONAL_LIGHT)
		std140Light.positionWS = glm::vec4(0);

	// Fill the rest of the info
	std140Light.color = glm::vec4(cpuLight.color, 1.0f);
	std140Light.attenuation = glm::vec4(cpuLight.attenuation, 0.0f);
	std140Light.dirWS = glm::vec4(
		glm::normalize(-cpuLight.transform.GetWorldObjectDirection()),
		0.0f
	);
	std140Light.spotlight = cpuLight.spotAnglesCos;
	std140Light.intensity = cpuLight.intensity;
	std140Light.type = static_cast<int>(cpuLight.type);

	return std140Light;
}

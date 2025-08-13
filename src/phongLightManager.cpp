#include "phongLightManager.h"

unsigned int PhongLightingManager::AddLight(const LightValues& lv)
{
    if (lv.type == LightcasterType::DIRECTIONAL_LIGHT) m_DirectionalLightCount++;
    else if (lv.type == LightcasterType::POINT_LIGHT)  m_PointLightCount++;
    else if (lv.type == LightcasterType::FLASH_LIGHT)  m_SpotLightCount++;
    m_Count++;
    m_LightValues.push_back(lv);
    return static_cast<unsigned int>(m_LightValues.size() - 1);
}

std::vector<LightValues>& PhongLightingManager::getAllLightValues()
{
	return m_LightValues;
}

#ifndef UBO_ARRAY_H
#define UBO_ARRAY_H

#include "uniformBuffer.h"
#include <vector>
#include <algorithm>

template<typename T, int N>
class UboArray
{
public:
	UboArray(unsigned int bindingUnit, std::string blockName) : m_bindingUnit(bindingUnit), m_count(N), uboName(blockName)
	{
		m_UBO.Bind(bindingUnit);
		// Inside each of the uniform .x of the first 16 byte section will be the number of structs
		m_UBO.AllocateData(sizeof(glm::uvec4) + sizeof(T) * N, GL_DYNAMIC_DRAW);
	}

	void UpdateData(const std::vector<T> data)
	{
		m_UBO.Bind(m_bindingUnit);

		// Get the size but clamp so it does not reach over the initially set size
		unsigned int size = std::min(static_cast<size_t>(m_count), data.size());
		
		// Update header first; Note the shader only cares about the .x value of this
		glm::uvec4 header = glm::uvec4(size, 0, 0, 0);
		m_UBO.UpdateData(sizeof(header), 0, &header);

		// Now for the actual data
		m_UBO.UpdateData(size * sizeof(T), sizeof(header), data.data());
	}

	void linkToShader(const Shader& shaderProgram)
	{
		m_UBO.AttachShader(shaderProgram, m_bindingUnit, uboName);
	}

private:
	UniformBuffer m_UBO;
	unsigned int m_bindingUnit;
	unsigned int m_count;
	std::string uboName;
};

#endif
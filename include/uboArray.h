#ifndef UBO_ARRAY_H
#define UBO_ARRAY_H

#include "uniformBuffer.h"
#include <vector>

template<typename T, int N>
class UboArray
{
public:
	UboArray(unsigned int bindingUnit) : m_bindingUnit(bindingUnit), m_count(N)
	{
		m_UBO.Bind(bindingUnit);
		m_UBO.AllocateData(sizeof(T) * N, GL_DYNAMIC_DRAW);
	}

	void UpdateData(const std::vector<T> data)
	{
		m_UBO.Bind(m_bindingUnit);
		
		// Get the size but clamp so it does not reach over the initially set size
		unsigned int size = std::min(m_count, data.size());
		m_UBO.UpdateData(size * sizeof(T), data.data());
	}

private:
	UniformBuffer m_UBO;
	unsigned int m_bindingUnit;
	unsigned int m_count;
};

#endif
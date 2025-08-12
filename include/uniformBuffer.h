#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <glad/glad.h>
#include "shader.h"

class UniformBuffer
{
public:
	UniformBuffer();
	~UniformBuffer() { glDeleteBuffers(1, &m_Id); };
	UniformBuffer(const UniformBuffer&) = delete;
	UniformBuffer& operator=(const UniformBuffer&) = delete;
	UniformBuffer(UniformBuffer&& o) noexcept : m_Id(o.m_Id), m_Size(o.m_Size) { o.m_Id = 0; o.m_Size = 0; }
	UniformBuffer& operator=(UniformBuffer&& o) noexcept {
		if (this != &o) {
			if (m_Id) glDeleteBuffers(1, &m_Id);
			m_Id = o.m_Id;  m_Size = o.m_Size;
			o.m_Id = 0;     o.m_Size = 0;
		}
		return *this;
	}

	void Bind(unsigned int bindingPoint);
	void AllocateData(size_t byteSize, unsigned int usage);
	void UpdateData(size_t byteSize, size_t dstOffset, const void* data);
	void AttachShader(const Shader& shaderProgram, unsigned int bindingBlock, std::string blockName);

private:
	unsigned int m_Id;
	size_t m_Size;
};

#endif
#ifndef UNIFORM_BUFFER_H
#define UNIFORM_BUFFER_H

#include <glad/glad.h>
#include "shader.h"

class UniformBuffer
{
public:
	UniformBuffer() {};
	~UniformBuffer() { glDeleteBuffers(1, &m_Id); };

	void Bind(unsigned int bindingPoint);
	void AllocateData(size_t byteSize, unsigned int usage);
	void UpdateData(size_t byteSize, const void* data);
	void AttachShader(const Shader& shaderProgram);

private:
	unsigned int m_Id;
	unsigned int m_Size;
};

#endif
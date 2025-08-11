#include "uniformBuffer.h"

void UniformBuffer::Bind(unsigned int bindingPoint)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Id);
}

void UniformBuffer::AllocateData(size_t byteSize, unsigned int usage)
{
	glGenBuffers(1, &m_Id);
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);

	// We do put data yet, just allocating space
	glBufferData(GL_UNIFORM_BUFFER, byteSize, nullptr, usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::UpdateData(size_t byteSize, const void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, byteSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::AttachShader(const Shader& shaderProgram)
{
	shaderProgram.use();
	unsigned int uboIdx = shaderProgram.getUniformBlockIndex("PhongLighting");
}

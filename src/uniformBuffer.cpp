#include "uniformBuffer.h"

UniformBuffer::UniformBuffer() : m_Size(0)
{
	glGenBuffers(1, &m_Id);
}

void UniformBuffer::Bind(unsigned int bindingPoint)
{
	glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_Id);
}

void UniformBuffer::AllocateData(size_t byteSize, unsigned int usage)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);

	// We do put data yet, just allocating space
	glBufferData(GL_UNIFORM_BUFFER, byteSize, nullptr, usage);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
	m_Size = byteSize;
}

void UniformBuffer::UpdateData(size_t byteSize, size_t dstOffset, const void* data)
{
	glBindBuffer(GL_UNIFORM_BUFFER, m_Id);
	glBufferSubData(GL_UNIFORM_BUFFER, (GLintptr)dstOffset, (GLsizeiptr)byteSize, data);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void UniformBuffer::AttachShader(const Shader& shaderProgram, unsigned int bindingBlock, const std::string& blockName)
{
	shaderProgram.use();
	unsigned int uboIdx = shaderProgram.getUniformBlockIndex(blockName.c_str());
	if (uboIdx != GL_INVALID_INDEX) shaderProgram.glBindBlockToBindingPoint(uboIdx, bindingBlock);
}

#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <VertexBuffer.h>
#include <VertexBufferLayout.h>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();
	VertexArray(const VertexArray&) = delete;
	VertexArray& operator=(const VertexArray&) = delete;
	VertexArray(VertexArray&&) noexcept = default;
	VertexArray& operator=(VertexArray&&) noexcept = default;

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:
	unsigned int m_Id;
};

#endif
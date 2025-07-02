#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <vertexBuffer.h>
#include <vertexBufferLayout.h>

class VertexArray
{
public:
	VertexArray();
	~VertexArray();

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:
	unsigned int m_Id;
};

#endif
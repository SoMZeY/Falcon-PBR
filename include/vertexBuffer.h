#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer(const void* data, unsigned int size, unsigned int usage);
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_Id;
};


#endif
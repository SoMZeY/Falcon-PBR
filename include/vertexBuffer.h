#ifndef VERTEXBUFFER_H
#define VERTEXBUFFER_H

#include <glad/glad.h>

class VertexBuffer
{
public:
	VertexBuffer() : m_Id(0) {};
	VertexBuffer(const void* data, unsigned int size, unsigned int usage);
	~VertexBuffer();
	VertexBuffer(const VertexBuffer&) = delete;
	VertexBuffer& operator=(const VertexBuffer&) = delete;

	VertexBuffer(VertexBuffer&& other) noexcept : m_Id(other.m_Id) {
		other.m_Id = 0;
	}

	VertexBuffer& operator=(VertexBuffer&& other) noexcept {
		if (this != &other) {
			reset();
			m_Id = other.m_Id;
			other.m_Id = 0;
		}
		return *this;
	}

	void reset() {
		if (m_Id) { glDeleteBuffers(1, &m_Id); m_Id = 0; }
	}

	void Bind() const;
	void Unbind() const;

private:
	unsigned int m_Id;
};


#endif
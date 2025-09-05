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

	VertexArray(VertexArray&& other) noexcept : m_Id(other.m_Id) {
		other.m_Id = 0;
	}

	VertexArray& operator=(VertexArray&& other) noexcept {
		if (this != &other) {
			reset();
			m_Id = other.m_Id;
			other.m_Id = 0;
		}
		return *this;
	}

	void reset() {
		if (m_Id) { glDeleteVertexArrays(1, &m_Id); m_Id = 0; }
	}

	void Bind() const;
	void Unbind() const;

	void AddBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
private:
	unsigned int m_Id;
};

#endif
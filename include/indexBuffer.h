#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <glad/glad.h>

class IndexBuffer
{
public:
	IndexBuffer() : m_Id(0), m_Count(0) {};
	IndexBuffer(const unsigned int* data, unsigned int count, unsigned int usage);
	~IndexBuffer();
	IndexBuffer(const IndexBuffer&) = delete;
	IndexBuffer& operator=(const IndexBuffer&) = delete;

	IndexBuffer(IndexBuffer&& other) noexcept : m_Id(other.m_Id) {
		other.m_Id = 0;
	}

	IndexBuffer& operator=(IndexBuffer&& other) noexcept {
		if (this != &other) {
			reset();
			m_Id = other.m_Id;
			m_Count = other.GetCount();
			other.m_Id = 0;
			other.m_Count = 0;
		}
		return *this;
	}

	void reset() {
		if (m_Id) {
			glDeleteBuffers(1, &m_Id);
			m_Id = 0;
			m_Count = 0; 
		}
	}

	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_Id;
	unsigned int m_Count;
};

#endif
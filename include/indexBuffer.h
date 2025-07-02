#ifndef INDEXBUFFER_H
#define INDEXBUFFER_H

#include <glad/glad.h>

class IndexBuffer
{
public:
	IndexBuffer(const unsigned int* data, unsigned int count, unsigned int usage);
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;

	inline unsigned int GetCount() const { return m_Count; }

private:
	unsigned int m_Id;
	unsigned int m_Count;
};

#endif
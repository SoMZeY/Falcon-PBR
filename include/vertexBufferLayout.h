#ifndef VERTEXBUFFERLAYOUT_H
#define VERTEXBUFFERLAYOUT_H

#include <glad/glad.h>
#include <vector>

struct VertexBufferElement
{
	unsigned int type;
	unsigned int count;
	unsigned int normalized;

	VertexBufferElement(unsigned int t, unsigned int c, unsigned int n) :
		count(c), type(t), normalized(n)
	{

	}

	static unsigned int GetSizeOfType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}

		return 0;
	}
};

class VertexBufferLayout
{
public:
	VertexBufferLayout()
		: m_Stride(0) {}
	VertexBufferLayout(const VertexBufferLayout&) = delete;
	VertexBufferLayout& operator=(const VertexBufferLayout&) = delete;
	VertexBufferLayout(VertexBufferLayout&&) noexcept = default;
	VertexBufferLayout& operator=(VertexBufferLayout&&) noexcept = default;

	template<typename T>
	void Push(unsigned int count) {
		static_assert(sizeof(T) == 0, "Unsupported type passed to VertexBufferLayout::Push.");
	}

	inline const std::vector<VertexBufferElement>& GetElements() const { return m_Elements; };
	inline unsigned int GetStride() const { return m_Stride; };
private:
	unsigned int m_Stride;
	std::vector<VertexBufferElement> m_Elements;
};

// Specialization for float
template<>
inline void VertexBufferLayout::Push<float>(unsigned int count)
{
	m_Elements.push_back(VertexBufferElement({ GL_FLOAT, count, GL_FALSE }));
	m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

// Specialization for unsigned int
template<>
inline void VertexBufferLayout::Push<unsigned int>(unsigned int count)
{
	m_Elements.push_back(VertexBufferElement({ GL_UNSIGNED_INT, count, GL_FALSE }));
	m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

// Specialization for unsigned char
template<>
inline void VertexBufferLayout::Push<unsigned char>(unsigned int count)
{
	m_Elements.push_back(VertexBufferElement({ GL_UNSIGNED_BYTE, count, GL_TRUE }));
	m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}

#endif
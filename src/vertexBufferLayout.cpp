#include "vertexBufferLayout.h"

VertexBufferLayout::VertexBufferLayout()
{
}

template<typename T>
void VertexBufferLayout::Push(int count)
{
	static_assert(sizeof(T) == 0, "Push<T> not implemented for this type.");
}

// Specialization for float
template<>
void VertexBufferLayout::Push<float>(int count)
{
	m_Elements.push_back(VertexBufferElement({ GL_FLOAT, count, GL_FALSE }));
	m_Stride += VertexBufferElement::GetSizeOfType(GL_FLOAT) * count;
}

// Specialization for unsigned int
template<>
void VertexBufferLayout::Push<unsigned int>(int count)
{
	m_Elements.push_back(VertexBufferElement({ GL_UNSIGNED_INT, count, GL_FALSE }));
	m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_INT) * count;
}

// Specialization for unsigned char
template<>
void VertexBufferLayout::Push<unsigned char>(int count)
{
	m_Elements.push_back(VertexBufferElement({ GL_UNSIGNED_BYTE, count, GL_TRUE }));
	m_Stride += VertexBufferElement::GetSizeOfType(GL_UNSIGNED_BYTE) * count;
}
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
		: m_Stride(0) {};

	template<typename T>
	void Push(int count);

	inline const std::vector<VertexBufferElement> GetElements() const { return m_Elements; };
	inline unsigned int GetStride() const { return m_Stride; };
private:
	unsigned int m_Stride;
	std::vector<VertexBufferElement> m_Elements;
};
#endif
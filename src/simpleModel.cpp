#include "simpleModel.h"
#include <iostream>

SimpleModel::SimpleModel(SimpleModels simpleModel)
{
	if (simpleModel == SimpleModels::CUBE)
	{
		float cubeVertices[48] = {
			// posX, posY, posZ,   normX, normY, normZ
			 1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f, // Top right  front
			 1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f, // Top right  back
			-1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f, // Top left   front
			-1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f, // Top left   back
			 1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f, // Bottom right front
			 1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f, // Bottom right back
			-1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f, // Bottom left  front
			-1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f  // Bottom left  back
		};

		unsigned int cubeIndices[36] = {
			// Top
			0, 2, 1,   1, 2, 3,

			// Bottom
			5, 6, 4,   7, 6, 5,

			// Right
			0, 1, 5,   0, 5, 4,

			// Left
			2, 6, 7,   2, 7, 3,

			// Front
			2, 0, 4,   2, 4, 6,

			// Back
			1, 3, 7,   1, 7, 5
		};

		// BUFFER FORMATTING
		m_Vbo = VertexBuffer(cubeVertices, 48 * sizeof(float), GL_STATIC_DRAW);
		m_Vbo.Bind();

		VertexBufferLayout cubeVaoLayout;
		cubeVaoLayout.Push<float>(3);
		cubeVaoLayout.Push<float>(3);

		m_Vao.Bind();
		m_Vao.AddBuffer(m_Vbo, cubeVaoLayout);

		m_Ibo = IndexBuffer(cubeIndices, 36, GL_STATIC_DRAW);
		m_Ibo.Bind();
	}
	else
	{
		std::cerr << "Error: Empty/Unknown simple model" << std::endl;
	}
}

void SimpleModel::Draw(Shader& shaderProgram) const
{
	m_Vbo.Bind();
	m_Vao.Bind();

	shaderProgram.use();
	glDrawElements(GL_TRIANGLES, m_Ibo.GetCount(), GL_UNSIGNED_INT, NULL);
}

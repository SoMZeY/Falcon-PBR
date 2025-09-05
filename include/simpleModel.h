#ifndef SIMPLE_MODEL_H
#define SIMPLE_MODEL_H

#include "model.h"
#include "vertexArray.h"
#include "vertexBuffer.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"

enum class SimpleModels
{
	MODEL_UNKNOWN = -1,
	CUBE = 0,
};

class SimpleModel : public Model
{
public:
	SimpleModel(SimpleModels simpleModel);
	void Draw(Shader& shaderProgram) const override;
private:
	VertexBuffer m_Vbo;
	VertexArray m_Vao;
	IndexBuffer m_Ibo;
};

#endif
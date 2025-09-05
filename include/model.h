#ifndef MODEL_H
#define MODEL_H

#include "shader.h"

class Model
{
public:
	virtual ~Model() = default;
	virtual void Draw(Shader& shaderProgram) const = 0;
};

#endif
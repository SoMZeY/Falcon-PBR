#include "transform.h"
#include "glm/gtc/matrix_transform.hpp"

Transform::Transform(float x, float y, float z)
{

	modelMatrix = glm::mat4(1.0f);
	modelMatrix = glm::translate(modelMatrix, glm::vec3(x, y, z));
}

Transform::Transform(const glm::mat4& model)
{
	modelMatrix = model;
}

glm::vec4 Transform::GetWorldLocation()
{
	return modelMatrix[3];
}

glm::mat4 Transform::GetModelMatrix()
{
	return modelMatrix;
}

glm::mat4 Transform::Rotate(float pitchRadians, float yawRadians, float rollRadians)
{
	if (pitchRadians)		modelMatrix = glm::rotate(modelMatrix, pitchRadians,	glm::vec3(1.0f, 0.0f, 0.0f));
	if (yawRadians)			modelMatrix = glm::rotate(modelMatrix, yawRadians,	glm::vec3(0.0f, 1.0f, 0.0f));
	if (rollRadians)		modelMatrix = glm::rotate(modelMatrix, rollRadians,	glm::vec3(0.0f, 0.0f, 1.0f));

	return modelMatrix;
}

glm::mat4 Transform::TranslateLocal(glm::vec3 translationVector)
{
	modelMatrix = glm::translate(modelMatrix, translationVector);
	return modelMatrix;
}

glm::mat4 Transform::TranslateLocal(glm::mat4 translationMatrix)
{
	modelMatrix = modelMatrix * translationMatrix;
	return modelMatrix;
}

glm::mat4 Transform::UniformScale(float scaleFactor)
{
	modelMatrix = glm::scale(modelMatrix, glm::vec3(scaleFactor));
	return modelMatrix;
}

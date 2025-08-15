#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/glm.hpp>


class Transform
{
public:
	Transform();
	Transform(float x, float y, float z);
	explicit Transform(const glm::mat4& model);
	
	glm::vec4 GetWorldLocation() const;
	glm::mat4 GetModelMatrix() const;
	glm::vec3 GetWorldObjectDirection() const;

	glm::mat4 Rotate(float pitch, float yaw, float roll);
	glm::mat4 TranslateLocal(glm::vec3 translationVector);
	glm::mat4 TranslateLocal(glm::mat4 translationMatrix);
	glm::mat4 UniformScale(float scaleFactor);
private:
	glm::mat4 modelMatrix;
};

#endif
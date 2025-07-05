#ifndef CAMERA_H
#define CAMERA_H

#include <glm/glm.hpp>

class Camera {

public:
	Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane);
	Camera(const Camera&) = delete;
	Camera& operator=(const Camera&) = delete;
	Camera(Camera&&) noexcept = default;
	Camera& operator=(Camera&&) noexcept = default;

	// Core functions
	glm::mat4 GetViewMatrix() const;
	glm::mat4 GetProjectionMatrix() const;

	// Position and Orientation
	glm::vec3 GetPosition() const;
	void SetPosition(const glm::vec3& newPosition);

	// Axis getters
	glm::vec3 GetForward() const;
	glm::vec3 GetUp() const;
	glm::vec3 GetRight() const;

	// Update internal orientation
	void LookAt(const glm::vec3& target);

	// Movement
	void Move(const glm::vec3& delta);
	void MoveRelative(const glm::vec3& delta);
	void Rotate(float pitch, float yaw);
private:
	// State
	glm::vec3 position;
	glm::vec3 forward, up, right;
	float yaw, pitch;

	// Transform matrices
	mutable glm::mat4 projectionMatrix;
	mutable glm::mat4 viewMatrix;

	// Flag to prevent unnecassary calculations
	mutable bool viewDirty;
	mutable bool projectionDirty;

	float fov, aspect, nearPlane, farPlane;

	// Used for recalculation of the forward, up, right vectors
	void UpdateCameraVectors();
};

#endif
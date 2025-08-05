#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>

#include <iostream>

Camera::Camera(float fovDegrees, float aspectRatio, float nearPlane, float farPlane)
	: fov(fovDegrees), aspect(aspectRatio),
	nearPlane(nearPlane), farPlane(farPlane),
	position(0.0f, 0.0f, 3.0f),
	pitch(0.0f), yaw(-90.0f),
	viewDirty(true), projectionDirty(true)
{
	UpdateCameraVectors();
}

glm::mat4 Camera::GetViewMatrix() const
{
	if (viewDirty)
	{
		viewMatrix = glm::lookAt(position, position + forward, up);
		viewDirty = false;
	}

	return viewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix() const
{
	if (projectionDirty)
	{
		projectionMatrix = glm::perspective(glm::radians(fov), aspect, nearPlane, farPlane);
		projectionDirty = false;
	}

	return projectionMatrix;
}

glm::vec3 Camera::GetPosition() const
{
	return position;
}

void Camera::SetPosition(const glm::vec3& newPosition)
{
	position = newPosition;
	viewDirty = true;
}

glm::vec3 Camera::GetForward() const
{
	return forward;
}

glm::vec3 Camera::GetUp() const
{
	return up;
}

glm::vec3 Camera::GetRight() const
{
	return right;
}

void Camera::LookAt(const glm::vec3& target)
{
	// Calculate
	forward = glm::normalize(target - position);
	right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(forward, right));

	pitch = glm::degrees(asin(forward.y));
	yaw = glm::degrees(atan2(forward.z, forward.x));
	
	viewDirty = true;
}

void Camera::Move(const glm::vec3& delta)
{
	position += delta;
	viewDirty = true;
}

void Camera::MoveRelative(const glm::vec3& delta)
{
	position += delta.x * right +
		delta.y * up +
		delta.z * forward;

	viewDirty = true;
}

void Camera::Rotate(float deltaPitch, float deltaYaw)
{
	pitch += deltaPitch;
	yaw += deltaYaw;

	if (pitch >  89.0f) pitch =  89.0f;
	if (pitch < -89.0f) pitch = -89.0f;

	UpdateCameraVectors();
	viewDirty = true;
}

void Camera::UpdateCameraVectors()
{
	glm::vec3 fwd;
	fwd.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
	fwd.y = sin(glm::radians(pitch));
	fwd.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
	forward = glm::normalize(fwd);

	right = glm::normalize(glm::cross(forward, glm::vec3(0.0f, 1.0f, 0.0f)));
	up = glm::normalize(glm::cross(forward, right));
}

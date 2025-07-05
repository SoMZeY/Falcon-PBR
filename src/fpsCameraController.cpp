#include "fpsCameraController.h"
#include "GLFW/glfw3.h"

FpsCameraController::FpsCameraController(Camera* cam, float speed, float sensivity)
	: camera(cam), moveSpeed(speed), mouseSensitivity(sensivity) {}

void FpsCameraController::OnMouseMove(float dx, float dy)
{
	camera->Rotate(dy * mouseSensitivity, -dx * mouseSensitivity);
}

void FpsCameraController::OnKeyPress(int key, int action)
{
	if (action == GLFW_PRESS) keysHeld.insert(key);
	else if (action == GLFW_RELEASE) keysHeld.erase(key);
}

void FpsCameraController::Update(float dt)
{
	glm::vec3 moveDir(0.0f);

	if (keysHeld.count(GLFW_KEY_W))			   moveDir	-= camera->GetForward();
	if (keysHeld.count(GLFW_KEY_S))			   moveDir	+= camera->GetForward();
	if (keysHeld.count(GLFW_KEY_A))			   moveDir	+= camera->GetRight();
	if (keysHeld.count(GLFW_KEY_D))			   moveDir	-= camera->GetRight();
	if (keysHeld.count(GLFW_KEY_SPACE))        moveDir	-= camera->GetUp();
	if (keysHeld.count(GLFW_KEY_LEFT_CONTROL)) moveDir  += camera->GetUp();

	if (glm::length(moveDir) > 0.0f)
		camera->MoveRelative(glm::normalize(moveDir) * moveSpeed * dt);
}

void FpsCameraController::SetMoveSpeed(float speed)
{
	moveSpeed = speed;
}

void FpsCameraController::SetMouseSensitivity(float sensitivity)
{
	mouseSensitivity = sensitivity;
}


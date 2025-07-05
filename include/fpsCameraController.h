#ifndef FPS_CAMERA_CONTROLLER_H
#define FPS_CAMERA_CONTROLLER_H

#include <iCameraController.h>
#include <camera.h>
#include <unordered_set>

class FpsCameraController : public CameraController
{
public:
	FpsCameraController(Camera* cam, float speed = 5.0f, float mouseSensitivity = 0.1f);
	FpsCameraController(const FpsCameraController&) = delete;
	FpsCameraController& operator=(const FpsCameraController&) = delete;
	FpsCameraController(FpsCameraController&&) noexcept = default;
	FpsCameraController& operator=(FpsCameraController&&) noexcept = default;

	void OnMouseMove(float dx, float dy) override;
	void OnKeyPress(int key, int action) override;
	void Update(float dt) override;

	void SetMoveSpeed(float speed);
	void SetMouseSensitivity(float sensitivity);

private:
	Camera* camera;
	float moveSpeed;
	float mouseSensitivity;
	std::unordered_set<int> keysHeld;
};

#endif 
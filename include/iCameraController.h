#ifndef I_CAMERA_CONTROLLER_H
#define I_CAMERA_CONTROLLER_H

#include <camera.h>

class CameraController
{
public:
	virtual void OnMouseMove(float dx, float dy) = 0;
	virtual void OnKeyPress(int key, int action) = 0;
	virtual void Update(float dt) = 0;
	virtual ~CameraController() {}
};

#endif
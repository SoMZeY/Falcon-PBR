#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "openglErrorReporting.h"
#include "vertexBuffer.h"
#include "vertexArray.h"
#include "vertexBufferLayout.h"
#include "indexBuffer.h"
#include "shader.h"
#include "fpsCameraController.h"
#include "gltfScene.h"
#include "phongLightManager.h"
#include "uboArray.h"
#include "transform.h"

#define MAX_AMOUNT_OF_PHONG_LIGHTS 16

struct PoorECS
{
	std::vector<GLTFScene*>		models;
	std::vector<Shader*>		shaderPrograms;
	std::vector<Transform*>		transforms;
};

class Renderer
{
public:
	Renderer(Camera* cameraPtr) : camera(cameraPtr), uboManager(0, "LightsUBO") {};
	~Renderer();
	void render();
	uint32_t InsertEntity(Shader* shaderProgram, GLTFScene* model, Transform* transform);
	int AddLightObject(const LightDesc& lightValues);
	void EditLightObject(int lightId, const LightDesc& lightValues);
	void DrawFloor();
private:
	void SyncLightsToViewSpace();
private:
	PhongLightingManager phong;
	const Camera* camera;
	PoorECS ecs;
	UboArray<LightWS, MAX_AMOUNT_OF_PHONG_LIGHTS> uboManager;
};

#endif
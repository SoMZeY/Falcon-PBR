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
#include "cubemap.h"
#include "simpleModel.h"

#define MAX_AMOUNT_OF_PHONG_LIGHTS 16

struct PoorECS
{
	std::vector<std::unique_ptr<Model>>			models;
	std::vector<std::unique_ptr<Shader>>		shaderPrograms;
	std::vector<std::unique_ptr<Transform>>		transforms;

	// For now its just for skyBox, so if skyBox then == true, not skyBox then == false. Later will make a struct for special instruction not just skyBox
	std::vector<bool>							specialInstr;
};

class Renderer
{
public:
	Renderer(Camera* cameraPtr) : camera(cameraPtr), uboManager(0, "LightsUBO"), skyBoxCubemap() {};
	~Renderer();
	void render();
	uint32_t InsertEntity(std::unique_ptr<Shader> shaderProgram, std::unique_ptr<Model> model, std::unique_ptr<Transform> transform, bool special);
	int AddLightObject(const LightDesc& lightValues);
	void EditLightObject(int lightId, const LightDesc& lightValues);

	void CreateFloor();
	void CreateSkyBox(const std::vector<std::string>& skyBoxImages);
private:
	void SyncLightsToViewSpace();
private:
	PhongLightingManager phong;
	Cubemap	skyBoxCubemap;
	const Camera* camera;
	PoorECS ecs;
	UboArray<LightWS, MAX_AMOUNT_OF_PHONG_LIGHTS> uboManager;
};

#endif
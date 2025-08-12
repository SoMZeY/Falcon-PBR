#ifndef RENDERER_H
#define RENDERER_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <openglErrorReporting.h>
#include <vertexBuffer.h>
#include <vertexArray.h>
#include <vertexBufferLayout.h>
#include <indexBuffer.h>
#include <shader.h>
#include <fpsCameraController.h>
#include <gltfScene.h>
#include <lightComponent.h>
#include <uboArray.h>

struct PoorECS
{
	std::vector<GLTFScene*>				models;
	std::vector<Shader*>				shaderPrograms;
	std::vector<PhongLightComponent*>   phongLights;
};

class Renderer
{
public:
	Renderer(Camera* cameraPtr) : camera(cameraPtr) {};
	~Renderer();
	void render();
	uint32_t InsertEntity(Shader* shaderProgram, GLTFScene* model, PhongLightComponent* phongLight);
private:
private:
	const Camera* camera;
	PoorECS ecs;
};


#endif
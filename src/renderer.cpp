#include "renderer.h"
#include <iostream>

Renderer::~Renderer()
{
}

void Renderer::render()
{
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = camera->GetProjectionMatrix();
	glm::mat4 model = glm::mat4(1.0f);

	for (int entityID = 0; entityID < ecs.models.size(); entityID++)
	{
		ecs.shaderPrograms[entityID]->use();
		glm::mat4 mvp = projection * view * model;

		// Pass in uniforms
		// Transform uniform
		ecs.shaderPrograms[entityID]->setMat4("u_MVP", mvp);
		ecs.shaderPrograms[entityID]->setMat4("u_Model", model);

		// Light uniform
		ecs.shaderPrograms[entityID]->setVec3("u_lightLocation", ecs.phongLights[entityID]->GetWorldPosition());
		ecs.shaderPrograms[entityID]->setFloat("u_lightIntensity", ecs.phongLights[entityID]->GetIntensity());
		ecs.shaderPrograms[entityID]->setVec3("u_lightColor", ecs.phongLights[entityID]->GetColor());
		ecs.shaderPrograms[entityID]->setVec3("u_cameraPos", camera->GetPosition());

		ecs.models[entityID]->Draw(*ecs.shaderPrograms[entityID]);
	}
}

uint32_t Renderer::InsertEntity(Shader* shaderProgram, GLTFScene* model, PhongLightComponent* phongLight)
{
	ecs.models.push_back(model);
	ecs.shaderPrograms.push_back(shaderProgram);
	ecs.phongLights.push_back(phongLight);

	return ecs.models.size() - 1;
}

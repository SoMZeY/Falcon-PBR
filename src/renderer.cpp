#include "renderer.h"
#include <iostream>

#define MAX_AMOUNT_OF_PHONG_LIGHTS 16

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

		// Light uniform object
		const size_t sizeOfPhongLights = ecs.phongLights.size();
		UboArray<LightValues, MAX_AMOUNT_OF_PHONG_LIGHTS> lightUbo(0, "LightsUBO");
		
		std::vector<LightValues> lights;

		for (int i = 0; i < MAX_AMOUNT_OF_PHONG_LIGHTS; i++)
			lights.push_back(ecs.phongLights[i]->getLightValues());

		lightUbo.UpdateData(lights);
		lightUbo.linkToShader(*ecs.shaderPrograms[entityID]);
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

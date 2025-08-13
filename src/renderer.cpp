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

    // Get lights once per frame
    const auto& lights = phong.getAllLightValues();

    for (size_t entityID = 0; entityID < ecs.models.size(); ++entityID)
    {
        Shader& shader = *ecs.shaderPrograms[entityID];
        shader.use();

        glm::mat4 mvp = projection * view * model;
        shader.setMat4("u_MVP", mvp);

        // Per-fragment view vector: pass camera position
        shader.setVec3("u_CameraPos", camera->GetPosition());

        // UBO upload + link
        UboArray<LightValues, MAX_AMOUNT_OF_PHONG_LIGHTS> ubo(0, "LightsUBO");
        ubo.UpdateData(lights);
        ubo.linkToShader(shader);

        ecs.models[entityID]->Draw(shader);
    }
}

uint32_t Renderer::InsertEntity(Shader* shaderProgram, GLTFScene* model)
{
	ecs.models.push_back(model);
	ecs.shaderPrograms.push_back(shaderProgram);

	return ecs.models.size() - 1;
}

void Renderer::AddLightObject(const LightValues& lightValues)
{
	phong.AddLight(lightValues);
}

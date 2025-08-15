#include "renderer.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

Renderer::~Renderer()
{
}

void Renderer::render()
{
    glm::mat4 view = camera->GetViewMatrix();
    glm::mat4 projection = camera->GetProjectionMatrix();

	// Sync the lights
	SyncLightsToViewSpace();

    for (size_t entityID = 0; entityID < ecs.models.size(); ++entityID)
    {
        Shader& shader = *ecs.shaderPrograms[entityID];
        shader.use();

		// THIS IS CUSTOM AND TEMPORARY BEHAVIOR =====START=====
		ecs.transforms[entityID]->Rotate(0.0f, 0.01f, 0.0f);
		// THIS IS CUSTOM AND TEMPORARY BEHAVIOR ======END======

		glm::mat4 modelView = view * ecs.transforms[entityID]->GetModelMatrix();
		glm::mat4 mvp = projection * modelView;
        shader.setMat4("u_MVP", mvp);
		shader.setMat4("u_ModelView", modelView);

        // Per-fragment view vector: pass camera position
        shader.setVec3("u_CameraPos", camera->GetPosition());
        shader.setVec3("u_ViewDir", camera->GetForward());

		// Link the ubo lights with the shader
		uboManager.linkToShader(shader);

        ecs.models[entityID]->Draw(shader);
    }

	DrawFloor();
}

uint32_t Renderer::InsertEntity(Shader* shaderProgram, GLTFScene* model, Transform* transform)
{
	ecs.models.push_back(model);
	ecs.shaderPrograms.push_back(shaderProgram);
	ecs.transforms.push_back(transform);

	return ecs.models.size() - 1;
}

int Renderer::AddLightObject(const LightDesc& lightValues)
{
	return phong.AddLight(lightValues);
}

void Renderer::EditLightObject(int lightId, const LightDesc& lightValues)
{
	// Edit the light manager's light by ID
	phong.EditLight(lightId, lightValues);
}

void Renderer::DrawFloor()
{
	float floor[48] = {
		// posX, posY, posZ,   normX, normY, normZ
		 1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f, // Top right  front
		 1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f, // Top right  back
		-1.0f,  1.0f,  1.0f,   0.0f, 1.0f, 0.0f, // Top left   front
		-1.0f,  1.0f, -1.0f,   0.0f, 1.0f, 0.0f, // Top left   back
		 1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f, // Bottom right front
		 1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f, // Bottom right back
		-1.0f, -1.0f,  1.0f,   0.0f, 1.0f, 0.0f, // Bottom left  front
		-1.0f, -1.0f, -1.0f,   0.0f, 1.0f, 0.0f  // Bottom left  back
	};

	unsigned int floorIdx[36] = {
		2, 0, 1,  2, 1, 3,
		6, 5, 4,  6, 7, 5,
		0, 1, 5,  0, 5, 4,
		2, 6, 7,  2, 7, 3,
		2, 0, 4,  2, 4, 6,
		1, 3, 7,  1, 7, 5
	};

	// BUFFER FORMATTING
	VertexBuffer floorVbo(floor, 48 * sizeof(float), GL_STATIC_DRAW);
	floorVbo.Bind();

	VertexBufferLayout floorVaoLayout;
	floorVaoLayout.Push<float>(3);
	floorVaoLayout.Push<float>(3);

	VertexArray floorVao;
	floorVao.Bind();
	floorVao.AddBuffer(floorVbo, floorVaoLayout);

	IndexBuffer floorIndexBuffer(floorIdx, 36, GL_STATIC_DRAW);
	floorIndexBuffer.Bind();
	
	// SHADER creation
	Shader floorShader((std::string(RESOURCES_PATH) + "shaders/vertexShader.vert").c_str(),
		(std::string(RESOURCES_PATH) + "shaders/fragmentShader.frag").c_str());

	floorShader.use();

	// MVP matrices
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = camera->GetProjectionMatrix();
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f));
	glm::mat4 modelView = view * model;
	glm::mat4 mvp = projection * modelView;

	// Attach to the uniform
	floorShader.setMat4("u_MVP", mvp);
	floorShader.setVec3("u_CameraPos", camera->GetPosition());
	floorShader.setVec3("u_ViewDir", camera->GetForward());
	floorShader.setMat4("u_ModelView", modelView);

	// Link the shader with the ubo manager
	uboManager.linkToShader(floorShader);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
}

void Renderer::SyncLightsToViewSpace()
{
	// Get the VIEW matrix
	glm::mat4 viewMatrix = camera->GetViewMatrix();

	// Get the lights in WORLD SPACE from the light manager
	std::vector<LightWS> lights;
	bool success = phong.getUboPhongLights(lights);

	// Update the necessary fields so they would be in the VIEW space
	for (LightWS& light : lights)
	{
		if (light.type != static_cast<int>(LightcasterType::DIRECTIONAL_LIGHT))
			light.positionWS = viewMatrix * light.positionWS;
		light.dirWS = glm::vec4(glm::normalize(glm::mat3(viewMatrix) * glm::vec3(light.dirWS)), 0.0f);
	}

	// Put the updated in VIEW space lights to the UBO
	uboManager.UpdateData(lights);
}

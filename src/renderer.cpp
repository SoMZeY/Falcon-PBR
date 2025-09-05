#include "renderer.h"
#include <iostream>

#include <glm/gtc/matrix_transform.hpp>

Renderer::~Renderer()
{
}

void Renderer::render()
{
	// Full View
	const glm::mat4 viewWorld = camera->GetViewMatrix();

	// View with no translation
	const glm::mat4 viewSkybox = glm::mat4(glm::mat3(viewWorld));

	const glm::mat4 projection = camera->GetProjectionMatrix();

	// Lights in view space use the full view
	SyncLightsToViewSpace();

	for (size_t id = 0; id < ecs.models.size(); ++id)
	{
		Shader& shader = *ecs.shaderPrograms[id];
		shader.use();

		shader.setVec3("u_CameraPos", camera->GetPosition());
		shader.setVec3("u_ViewDir", camera->GetForward());

		if (ecs.specialInstr[id]) 
		{
			// Depth: test but don't write; allow depth==1 to pass
			glDepthMask(GL_FALSE);
			glDepthFunc(GL_LEQUAL);

			glm::mat4 modelView = viewSkybox * ecs.transforms[id]->GetModelMatrix();
			glm::mat4 mvp = projection * modelView;
			shader.setMat4("u_MVP", mvp);
			shader.setMat4("u_ModelView", modelView);

			skyBoxCubemap.BindTexture();
			ecs.models[id]->Draw(shader);

			// Restore for normal objects
			glDepthFunc(GL_LESS);
			glDepthMask(GL_TRUE);
		}
		else 
		{
			glm::mat4 modelView = viewWorld * ecs.transforms[id]->GetModelMatrix();
			glm::mat4 mvp = projection * modelView;
			shader.setMat4("u_MVP", mvp);
			shader.setMat4("u_ModelView", modelView);

			uboManager.linkToShader(shader);
			ecs.models[id]->Draw(shader);
		}
	}
}

uint32_t Renderer::InsertEntity(std::unique_ptr<Shader> shaderProgram, std::unique_ptr<Model> model, std::unique_ptr<Transform> transform, bool special)
{
	ecs.models.push_back(std::move(model));
	ecs.shaderPrograms.push_back(std::move(shaderProgram));
	ecs.transforms.push_back(std::move(transform));
	ecs.specialInstr.push_back(special);

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

void Renderer::CreateFloor()
{
	std::unique_ptr<SimpleModel> floorCube = std::make_unique<SimpleModel>(SimpleModels::CUBE);
	// SHADER creation
	std::unique_ptr<Shader> floorShader = std::make_unique<Shader>((std::string(RESOURCES_PATH) + "shaders/vertexShader.vert").c_str(),
		(std::string(RESOURCES_PATH) + "shaders/fragmentShader.frag").c_str());

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));

	std::unique_ptr<Transform> floorTransform = std::make_unique<Transform>(model);
	InsertEntity(std::move(floorShader), std::move(floorCube), std::move(floorTransform), false);
}

void Renderer::CreateSkyBox(const std::vector<std::string>& skyBoxImages)
{
	skyBoxCubemap.LoadTextures(skyBoxImages);

	std::unique_ptr<SimpleModel> skyBoxCube = std::make_unique<SimpleModel>(SimpleModels::CUBE);

	std::unique_ptr<Shader> skyBoxShader = std::make_unique<Shader>((std::string(RESOURCES_PATH) + "shaders/skyBoxVertex.vert").c_str(),
		(std::string(RESOURCES_PATH) + "shaders/skyBoxFragment.frag").c_str());

	std::unique_ptr<Transform> skyBoxTransform = std::make_unique<Transform>(glm::mat4(1.0f));
	InsertEntity(std::move(skyBoxShader), std::move(skyBoxCube), std::move(skyBoxTransform), true);
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

		if (light.type == static_cast<int>(LightcasterType::SPOT_LIGHT))
			light.dirWS = -light.dirWS;
			//std::cout << "Spot light direction coord: x: " << light.dirWS.x << " y: " << light.dirWS.y << " z:" << light.dirWS.z << std::endl;
	}

	// Put the updated in VIEW space lights to the UBO
	uboManager.UpdateData(lights);
}

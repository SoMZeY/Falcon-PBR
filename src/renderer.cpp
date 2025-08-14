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

    // Get lights once per frame
    const auto& lights = phong.getAllLightValues();

    for (size_t entityID = 0; entityID < ecs.models.size(); ++entityID)
    {
        Shader& shader = *ecs.shaderPrograms[entityID];
        shader.use();

		ecs.transforms[entityID]->Rotate(0.0f, 0.01f, 0.0f);
        glm::mat4 mvp = projection * view * ecs.transforms[entityID]->GetModelMatrix();
        shader.setMat4("u_MVP", mvp);

        // Per-fragment view vector: pass camera position
        shader.setVec3("u_CameraPos", camera->GetPosition());
        shader.setVec3("u_ViewDir", camera->GetForward());

        // UBO upload + link
        ubo.UpdateData(lights);
        ubo.linkToShader(shader);

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

void Renderer::AddLightObject(const LightValues& lightValues)
{
	phong.AddLight(lightValues);
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

	Shader floorShader((std::string(RESOURCES_PATH) + "shaders/vertexShader.vert").c_str(),
		(std::string(RESOURCES_PATH) + "shaders/fragmentShader.frag").c_str());

	floorShader.use();

	// MVP matrices
	glm::mat4 view = camera->GetViewMatrix();
	glm::mat4 projection = camera->GetProjectionMatrix();
	
	// Model
	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // lower the cube
	model = glm::scale(model, glm::vec3(100.0f, 1.0f, 100.0f)); // wide, thin floor

	glm::mat4 mvp = projection * view * model;

	// Attach to the uniform
	floorShader.setMat4("u_MVP", mvp);

	// Per-fragment view vector: pass camera position
	floorShader.setVec3("u_CameraPos", camera->GetPosition());
	floorShader.setVec3("u_ViewDir", camera->GetForward());

	const auto& lights = phong.getAllLightValues();

	// UBO upload + link
	ubo.UpdateData(lights);
	ubo.linkToShader(floorShader);

	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, NULL);
}

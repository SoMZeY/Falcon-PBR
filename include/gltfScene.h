#ifndef GLTF_SCENE_H
#define GLTF_SCENE_H

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <vertexArray.h>
#include <vertexBuffer.h>
#include <indexBuffer.h>
#include "material.h"
#include "gltfTextureCache.h"
#include "model.h"

class GLTFScene : public Model
{
public:
	GLTFScene(const std::string& filename);
	void Draw(Shader& shaderProgram) const override;

private:
	void LoadFromFile(const std::string& path);
	void LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);
	void LoadMaterial(const tinygltf::Model& model, uint32_t materialIndx);

	struct PrimitiveRange
	{
		unsigned int firstIndex = 0;
		unsigned int indexCount = 0;
		int			 baseVertex = 0;
		int			 material   = -1;
	};

	// Keep the per primitve draw approach for now
	// TODO later: collapse into single draw call for each mesh
	struct Mesh
	{
		std::unique_ptr<VertexArray>	vao;
		std::unique_ptr<VertexBuffer>	vbo;
		std::unique_ptr<IndexBuffer>	ibo;
		std::vector<PrimitiveRange>		draws;

		//void render(const std::vector<Material>& mats) const;
		// Temporary until the materials are implemented
		void render() const;
	};
	std::vector<Mesh> meshes;
	std::vector<Material> materials;
	tinygltf::Model model;
	GltfTextureCache textureCache;
};

#endif
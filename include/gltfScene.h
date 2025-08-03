#ifndef GLTF_SCENE_H
#define GLTF_SCENE_H

// TODO later: figure this out
#define TINYGLTF_NO_STB_IMAGE_WRITE

#include <string>
#include <vector>
#include <memory>
#include <glm/glm.hpp>
#include <tiny_gltf.h>
#include <vertexArray.h>
#include <vertexBuffer.h>
#include <indexBuffer.h>


class GLTFScene
{
public:
	GLTFScene(const std::string& filename);
	void Draw() const;

private:
	void LoadFromFile(const std::string& path);
	void LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh);
	void LoadMaterial(const tinygltf::Model& model, const tinygltf::Material& material);

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
	//std::vector<Material> materials;
	tinygltf::Model model;
};

#endif
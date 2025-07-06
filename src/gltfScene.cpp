#include "gltfScene.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

GLTFScene::GLTFScene(const std::string& filename)
{
	LoadFromFile(filename);
}

GLTFScene::~GLTFScene()
{
	// TODO
}

void GLTFScene::Draw() const
{
}

void GLTFScene::LoadFromFile(const std::string& path)
{
	tinygltf::TinyGLTF loader;
	std::string err, warn;

	// Parse the file to see if binary or not
	bool binary = path.substr(path.size() - 4) == ".glb";
	bool sucess = binary ?
		loader.LoadBinaryFromFile(&model, &err, &warn, path) :
		loader.LoadASCIIFromFile(&model, &err, &warn, path);

	// Output the warnings/errors messages
	if (!warn.empty()) std::cout << "GLTF Warning: " << warn << std::endl;
	if (!err.empty())   std::cout << "GLTF Error: "   << err  << std::endl;

	// If did not succeed throw an error. TODO: change?
	if (!sucess)
		throw std::runtime_error("Failed to load GLTF file: " + path);

	// Load Materials and Meshes
	for (const auto& material : model.materials) LoadMaterial(model, material);
	for (const auto& mesh : model.meshes)    LoadMesh(model, mesh);
}

void GLTFScene::LoadMesh(const tinygltf::Model& model, const tinygltf::Mesh& mesh)
{
	// Pre scan to know total sizes
	unsigned int totalVertices = 0;
	unsigned int totalIndices = 0;

	// Temporary struct to help tracking metadata per primitive
	struct PrimMeta
	{
		const tinygltf::Primitive* primitive;
		size_t vertexCount;
		size_t indexCount;

		PrimMeta(const tinygltf::Primitive* prim, size_t vCount, size_t iCount)
		{
			primitive = prim;
			vertexCount = vCount;
			indexCount = iCount;
		}
	};
	
	// Have a vector of all primtives metadata per mesh
	std::vector<PrimMeta> tmp;

	// Populate all of the primitives' metadat
	for (const auto& primitive : mesh.primitives)
	{
		// Get the position metadata and index metadata
		const auto& posAcc = model.accessors.at(primitive.attributes.at("POSITION"));
		const auto& idxAcc = model.accessors.at(primitive.indices);

		// Add to the tmp vector and save the total number of vertices and indeces
		tmp.push_back(PrimMeta( &primitive, posAcc.count, idxAcc.count ));
		totalVertices += posAcc.count;
		totalIndices += idxAcc.count;
	}

	// Contiguos arrays
	// Interleaved (pos|norm|uv)
	std::vector<float> vertexData;
	std::vector<unsigned int> indexData;

	// To prevent multiple reallocations, reserve the capacity here
	// vertexData = (pos|norm|uv) ...
	// So 3 floats for pos, 3 floats for norm, and 2 floats for uv
	vertexData.reserve(totalVertices * 8);
	// Reserve capacity for the total amount of indexes
	indexData.reserve(totalIndices);

	// Assume that the first primitive's attributes are representative of the rest
	const bool hasNormal = tmp.front().primitive->attributes.count("NORMAL");
	const bool hasUV = tmp.front().primitive->attributes.count("TEXCOORD_0");
	const unsigned int stride = 3 + (hasNormal ? 3 : 0) + (hasUV ? 2 : 0);
	
	// Vertices packed so far
	unsigned int runningVertexBase = 0;

	// Pack each primitive
	Mesh mesh;
	for (const auto& primMeta : tmp)
	{
		const auto& prim = *primMeta.primitive;

		// ATTRIBUTE POINTERS
		const auto& posAcc = model.accessors.at(prim.attributes.at("POSITION"));
		const auto& posView = model.bufferViews.at(posAcc.bufferView);
		const auto& posBuf = model.buffers.at(posView.buffer);
		const float* posPtr = reinterpret_cast<const float*>(&posBuf.data[posView.byteOffset + posAcc.byteOffset]);
	}
}

void GLTFScene::LoadMaterial(const tinygltf::Model& model, const tinygltf::Material& material)
{
	//TODO
}

void GLTFScene::Mesh::render() const
{
}

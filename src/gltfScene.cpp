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
	bool success = binary ?
		loader.LoadBinaryFromFile(&model, &err, &warn, path) :
		loader.LoadASCIIFromFile(&model, &err, &warn, path);

	// Output the warnings/errors messages
	if (!warn.empty())  std::cout << "GLTF Warning: " << warn << std::endl;
	if (!err.empty())   std::cout << "GLTF Error: "   << err  << std::endl;

	// If did not succeed throw an error. TODO: change?
	if (!success)
		throw std::runtime_error("Failed to load GLTF file: " + path);

	// Load Materials and Meshes
	for (const auto& material : model.materials) LoadMaterial(model, material);
	for (const auto& mesh : model.meshes)    LoadMesh(model, mesh);
}

// TODO: create some kind of error handling. Currently the function is exception prone
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

	// Populate all of the primitives
	for (const tinygltf::Primitive& primitive : mesh.primitives)
	{
		// Get the position metadata and index metadata
		const auto& posAcc = model.accessors.at(primitive.attributes.at("POSITION"));
		const auto& idxAcc = model.accessors.at(primitive.indices);

		// Add to the tmp vector and save the total number of vertices and indeces
		tmp.push_back(PrimMeta( &primitive, posAcc.count, idxAcc.count ));
		totalVertices += posAcc.count;
		totalIndices += idxAcc.count;
	}

	// Interleaved data (pos|norm|uv)
	std::vector<float> vertexData;
	std::vector<unsigned int> indexData;

	// To prevent multiple reallocations, reserve the capacity here
	// So 3 floats for pos, 3 floats for norm, and 2 floats for uv
	vertexData.reserve(totalVertices * 8);

	// Reserve capacity for the total amount of indexes
	indexData.reserve(totalIndices);

	// TODO later: Add guard
	// Assume that the first primitive's attributes are representative of the rest
	const bool hasNormal = tmp.front().primitive->attributes.count("NORMAL");
	const bool hasUV = tmp.front().primitive->attributes.count("TEXCOORD_0");
	const unsigned int stride = 3 + (hasNormal ? 3 : 0) + (hasUV ? 2 : 0);
	
	// Vertices packed so far
	unsigned int runningVertexBase = 0;

	// Pack each primitive
	Mesh Cmesh;
	for (const auto& primMeta : tmp)
	{
		const tinygltf::Primitive &prim = *primMeta.primitive;

		// Define the pos, norm and UV data
		const float* posPtr  = nullptr;
		const float* normPtr = nullptr;
		const float* uvPtr   = nullptr;

		// Save the bufferView strides for each
		uint32_t posViewStride  = 0;
		uint32_t normViewStride = 0;
		uint32_t uvViewStride   = 0;

		// Resolve and reinterpret POSITION attribute into float pointer
		const auto& posAcc  = model.accessors.at(prim.attributes.at("POSITION"));
		const auto& posView = model.bufferViews.at(posAcc.bufferView);
		const auto& posBuf  = model.buffers.at(posView.buffer);
		posViewStride = posView.byteStride;
		if (posAcc.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && posAcc.type == TINYGLTF_TYPE_VEC3)
			posPtr = reinterpret_cast<const float*>(&posBuf.data[posView.byteOffset + posAcc.byteOffset]);
		else {} // TODO; handle the case

		// If NORMAL exist then Resolve and reinterpret cast into float
		if (hasNormal)
		{
			const auto& normAcc = model.accessors.at(prim.attributes.at("NORMAL"));
			const auto& normView = model.bufferViews.at(normAcc.bufferView);
			const auto& normBuf = model.buffers.at(normView.buffer);
			normViewStride = normView.byteStride;

			if (normAcc.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && normAcc.type == TINYGLTF_TYPE_VEC3)
				normPtr = reinterpret_cast<const float*>(&normBuf.data[normView.byteOffset + normAcc.byteOffset]);
			else {} // TODO; handle the case
		}

		// If UV exist then Resolve and reinterpret cast into float
		if (hasUV)
		{
			const auto& uvAcc  = model.accessors.at(prim.attributes.at("TEXCOORD_0"));
			const auto& uvView = model.bufferViews.at(uvAcc.bufferView);
			const auto& uvBuf  = model.buffers.at(uvView.buffer);
			uvViewStride = uvView.byteStride;
			if (uvAcc.componentType == TINYGLTF_COMPONENT_TYPE_FLOAT && uvAcc.type == TINYGLTF_TYPE_VEC2)
				uvPtr = reinterpret_cast<const float*>(&uvBuf.data[uvView.byteOffset + uvAcc.byteOffset]);
			else {} // TODO; handle the case
		}

		// Interleave into vertex data
		for (uint32_t i = 0; i < posAcc.count; i++)
		{
			// POSITION DATA
			uint32_t bStride;
			if (posPtr)
			{
				assert(posViewStride % 4 == 0);
				bStride = (posViewStride == 0) ? 3 : posViewStride / 4;
				vertexData.push_back(posPtr[i * bStride + 0]);
				vertexData.push_back(posPtr[i * bStride + 1]);
				vertexData.push_back(posPtr[i * bStride + 2]);
			}

			// Normals
			if (normPtr)
			{
				assert(normViewStride % 4 == 0);
				bStride = (normViewStride == 0) ? 3 : normViewStride / 4;
				vertexData.push_back(normPtr[i * bStride + 0]);
				vertexData.push_back(normPtr[i * bStride + 1]);
				vertexData.push_back(normPtr[i * bStride + 2]);
			}
			
			// UV
			if (uvPtr)
			{
				assert(uvViewStride % 4 == 0);
				bStride = (uvViewStride == 0) ? 2 : uvViewStride / 4;
				vertexData.push_back(uvPtr[i * bStride + 0]);
				vertexData.push_back(uvPtr[i * bStride + 1]);
			}
		}

		runningVertexBase += posAcc.count;
	}
}

void GLTFScene::LoadMaterial(const tinygltf::Model& model, const tinygltf::Material& material)
{
	//TODO
}

void GLTFScene::Mesh::render() const
{
}

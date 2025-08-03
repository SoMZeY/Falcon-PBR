#include "gltfScene.h"
#include <iostream>
#include <glm/gtc/type_ptr.hpp>

GLTFScene::GLTFScene(const std::string& filename)
{
	LoadFromFile(filename);
}

void GLTFScene::Draw() const
{
    for (const Mesh& mesh : meshes)
    {
        mesh.vbo->Bind();
        mesh.vao->Bind();
        for (const PrimitiveRange& prim : mesh.draws)
        {
            glDrawElementsBaseVertex(GL_TRIANGLES, prim.indexCount, GL_UNSIGNED_INT, reinterpret_cast<void*>(sizeof(uint32_t) * prim.firstIndex), prim.baseVertex);
        }
    }
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
	//for (const auto& material : model.materials) LoadMaterial(model, material);
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
            : primitive{ prim }, vertexCount{ vCount }, indexCount{ iCount } {
        }
    };

    // Have a vector of all primtives metadata per mesh
    std::vector<PrimMeta> tmp;

    // Populate all of the primitives
    for (const tinygltf::Primitive& primitive : mesh.primitives)
    {
        const auto& posAcc = model.accessors.at(primitive.attributes.at("POSITION"));
        const auto& idxAcc = model.accessors.at(primitive.indices);

        tmp.emplace_back(&primitive, posAcc.count, idxAcc.count);
        totalVertices += posAcc.count;
        totalIndices += idxAcc.count;
    }

    // Interleaved data (pos|norm|uv)
    std::vector<float>    vertexData;  vertexData.reserve(totalVertices * 8); // 3+3+2 max
    std::vector<uint32_t> indexData;   indexData.reserve(totalIndices);

    // Check which attributes exist across the whole mesh
    bool hasNormal = false, hasUV = false;
    for (auto& t : tmp)
    {
        hasNormal |= t.primitive->attributes.count("NORMAL");
        hasUV |= t.primitive->attributes.count("TEXCOORD_0");
    }
    const unsigned int stride = 3 + (hasNormal ? 3 : 0) + (hasUV ? 2 : 0);

    // Vertices / indices packed so far
    unsigned int runningVertexBase = 0;
    unsigned int runningIndexBase = 0;

    // Pack each primitive -----------------------------------------------------------------------
    Mesh Cmesh;
    for (const auto& primMeta : tmp)
    {
        const tinygltf::Primitive& prim = *primMeta.primitive;

        // Resolve POSITION
        const auto& posAcc = model.accessors.at(prim.attributes.at("POSITION"));
        const auto& posView = model.bufferViews.at(posAcc.bufferView);
        const auto& posBuf = model.buffers.at(posView.buffer);
        const float* posPtr = reinterpret_cast<const float*>(
            &posBuf.data[posView.byteOffset + posAcc.byteOffset]);

        // Resolve NORMAL (if any)
        const float* normPtr = nullptr;
        const auto* normAccPtr = prim.attributes.count("NORMAL") ?
            &model.accessors.at(prim.attributes.at("NORMAL")) : nullptr;
        if (normAccPtr)
        {
            const auto& normView = model.bufferViews.at(normAccPtr->bufferView);
            const auto& normBuf = model.buffers.at(normView.buffer);
            normPtr = reinterpret_cast<const float*>(
                &normBuf.data[normView.byteOffset + normAccPtr->byteOffset]);
        }

        // Resolve TEXCOORD_0 (if any)
        const float* uvPtr = nullptr;
        const auto* uvAccPtr = prim.attributes.count("TEXCOORD_0") ?
            &model.accessors.at(prim.attributes.at("TEXCOORD_0")) : nullptr;
        if (uvAccPtr)
        {
            const auto& uvView = model.bufferViews.at(uvAccPtr->bufferView);
            const auto& uvBuf = model.buffers.at(uvView.buffer);
            uvPtr = reinterpret_cast<const float*>(
                &uvBuf.data[uvView.byteOffset + uvAccPtr->byteOffset]);
        }

        // Index buffer
        const auto& idxAcc = model.accessors.at(prim.indices);
        const auto& idxView = model.bufferViews.at(idxAcc.bufferView);
        const auto& idxBuf = model.buffers.at(idxView.buffer);
        const uint8_t* idxRaw = &idxBuf.data[idxView.byteOffset + idxAcc.byteOffset];

        // Helper lambdas ------------------------------------------------------------------------
        auto floatStep = [](uint32_t byteStride, uint32_t elems)
            {
                return byteStride ? byteStride / sizeof(float) : elems;
            };
        const uint32_t stepPos = floatStep(posView.byteStride, 3);
        const uint32_t stepNorm = normAccPtr ? floatStep(model.bufferViews.at(normAccPtr->bufferView).byteStride, 3) : 0;
        const uint32_t stepUV = uvAccPtr ? floatStep(model.bufferViews.at(uvAccPtr->bufferView).byteStride, 2) : 0;

        // Record draw info ----------------------------------------------------------------------
        PrimitiveRange pr;
        pr.firstIndex = runningIndexBase;
        pr.indexCount = idxAcc.count;
        pr.baseVertex = runningVertexBase;
        //pr.materialIdx = prim.material;
        Cmesh.draws.push_back(pr);

        // Write vertices ------------------------------------------------------------------------
        for (uint32_t i = 0; i < posAcc.count; ++i)
        {
            // position (always there)
            vertexData.insert(vertexData.end(), posPtr + i * stepPos, posPtr + i * stepPos + 3);

            // normal
            if (hasNormal)
            {
                if (normPtr)
                    vertexData.insert(vertexData.end(), normPtr + i * stepNorm, normPtr + i * stepNorm + 3);
                else
                    vertexData.insert(vertexData.end(), { 0.f, 0.f, 0.f });
            }

            // uv
            if (hasUV)
            {
                if (uvPtr)
                    vertexData.insert(vertexData.end(), uvPtr + i * stepUV, uvPtr + i * stepUV + 2);
                else
                    vertexData.insert(vertexData.end(), { 0.f, 0.f });
            }
        }

        // Write indices -------------------------------------------------------------------------
        auto pushIndex = [&](uint32_t v) { indexData.push_back(v + runningVertexBase); };
        if (idxAcc.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
        {
            const uint8_t* src = reinterpret_cast<const uint8_t*>(idxRaw);
            for (size_t k = 0; k < idxAcc.count; ++k) pushIndex(src[k]);
        }
        else if (idxAcc.componentType == TINYGLTF_COMPONENT_TYPE_UNSIGNED_SHORT)
        {
            const uint16_t* src = reinterpret_cast<const uint16_t*>(idxRaw);
            for (size_t k = 0; k < idxAcc.count; ++k) pushIndex(src[k]);
        }
        else // UNSIGNED_INT
        {
            const uint32_t* src = reinterpret_cast<const uint32_t*>(idxRaw);
            for (size_t k = 0; k < idxAcc.count; ++k) pushIndex(src[k]);
        }

        runningVertexBase += posAcc.count;
        runningIndexBase += idxAcc.count;
    }

    // Populate GPU objects ----------------------------------------------------------------------
    Cmesh.vbo = std::make_unique<VertexBuffer>(vertexData.data(),
        vertexData.size() * sizeof(float), GL_STATIC_DRAW);

    VertexBufferLayout vbl;
    vbl.Push<float>(3);
    if (hasNormal) vbl.Push<float>(3);
    if (hasUV)     vbl.Push<float>(2);

    Cmesh.vao = std::make_unique<VertexArray>();
    Cmesh.vao->AddBuffer(*Cmesh.vbo, vbl);

    Cmesh.ibo = std::make_unique<IndexBuffer>(indexData.data(),
        indexData.size(), GL_STATIC_DRAW);

    meshes.push_back(std::move(Cmesh));
}

void GLTFScene::LoadMaterial(const tinygltf::Model& model, const tinygltf::Material& material)
{
	//TODO
}

void GLTFScene::Mesh::render() const
{
}

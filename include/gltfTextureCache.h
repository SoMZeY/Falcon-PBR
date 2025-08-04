#ifndef TEXTURE_CACHE_H
#define TEXTURE_CACHE_H

// TODO later: figure this out
#define TINYGLTF_NO_STB_IMAGE_WRITE

#include <unordered_map>
#include "texture.h"
#include "tiny_gltf.h"
#include <memory>

class GltfTextureCache
{
public:
	// Model specific approach
	std::shared_ptr<Texture> getTextureId(const tinygltf::Model& model, int32_t imageIdx);

private:
	std::unordered_map<int, std::shared_ptr<Texture>> textures;
};

#endif
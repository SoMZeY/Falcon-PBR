#include "gltfTextureCache.h"
#include "glad/glad.h"

#include <iostream>

std::shared_ptr<Texture> GltfTextureCache::getTextureId(const tinygltf::Model& model, int32_t imageIdx)
{
	// If already in the cache return a handle to that texture
	if (textures.count(imageIdx))
		return textures.at(imageIdx);

	// Since we already know the imageIdx we can go straight into it
	const tinygltf::Image& image = model.images.at(imageIdx);
	
	// Variables that will be used in creation of texture
	uint32_t width;
	uint32_t height;
	uint32_t format;
	uint32_t intFormat;
	uint32_t type;

	// Get the source format
	if (image.component == 4) // RGBA
		format = GL_RGBA;
	else if (image.component == 3) // RGB
		format = GL_RGB;
	else
	{
		std::cerr << "Unkown image format!" << std::endl;
		// TODO way later: implement a better exception handler
		return nullptr;
	}

	// Get the internal format
	intFormat = (format == GL_RGBA) ? GL_SRGB8_ALPHA8 : GL_SRGB8;

	// Get the type
	if (image.pixel_type == TINYGLTF_COMPONENT_TYPE_UNSIGNED_BYTE)
		type = GL_UNSIGNED_BYTE;
	else if (image.pixel_type == TINYGLTF_COMPONENT_TYPE_FLOAT)
		type = GL_FLOAT;
	else
	{
		std::cerr << "Unkown image data type!" << std::endl;
	}

	// Get the height and width
	width = image.width;
	height = image.height;

	// Create the texture
	std::shared_ptr<Texture> texture = std::make_shared<Texture>(width, height, intFormat, format, type, image.image.data());
	textures.insert({ imageIdx, texture });

	return texture;
}

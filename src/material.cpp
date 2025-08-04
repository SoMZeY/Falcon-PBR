#include "material.h"

//void Material::addTexture(const Texture* texture, const std::string& textureTypeName)
//{
//	textures.insert({ textureTypeName, texture });
//}

void Material::Bind(Shader& shaderProgram, int textureUnit) const
{
	shaderProgram.use();

	// We can just assume that all of them shader programs passed in will have aTexture
	shaderProgram.setInt("aTexture", textureUnit);
	texture->Bind(textureUnit);
}

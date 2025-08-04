#ifndef MATERIALS_H
#define MATERIALS_H

#include "texture.h"
#include "shader.h"
#include <string>
#include <unordered_map>
#include <memory>

class Material
{
public:
	Material(std::shared_ptr<Texture> texturePtr) : texture(texturePtr) {};
	//Material(const Shader* shader) : shaderProgram(shader) {};
	//void addTexture(const Texture* texture, const std::string& textureTypeName);

	// Needs to bind texture, set uniforms, and etc
	void Bind(Shader &shaderProgram, int textureUnit) const;

private:
	std::shared_ptr<Texture> texture;
};


#endif
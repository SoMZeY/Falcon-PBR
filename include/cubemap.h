#ifndef CUBEMAP_H
#define CUBEMAP_H

#include <vector>
#include <string>

class Cubemap
{
public:
	//Cubemap(unsigned int width,
	//	unsigned int height,
	//	unsigned int intFormat,
	//	unsigned int dataType,
	//	const std::vector<const unsigned char*> imagesData);

	Cubemap();

	// Needs to be in the format 
	// Right face
	// Left face
	// Top face
	// Bottom face
	// Back face
	// Front face
	Cubemap(const std::vector<std::string>& imagesPath);

	~Cubemap();
	void BindTexture();

	// Needs to be in the format 
	// Right face
	// Left face
	// Top face
	// Bottom face
	// Back face
	// Front face
	// If using this, we assume you want to discard all the exisiting texture images (if they exist)
	void LoadTextures(const std::vector<std::string>& imagesPath);
private:
	unsigned int m_Id;
};

#endif
#include "cubemap.h"
#include <glad/glad.h>

#include <stb_image.h>
#include <iostream>
#include <assert.h>

//Cubemap::Cubemap(unsigned int width, unsigned int height, unsigned int intFormat, unsigned int dataType, const std::vector<const unsigned char*> imagesData)
//{
//	
//}

Cubemap::Cubemap()
{
	glGenTextures(1, &m_Id);
}

Cubemap::Cubemap(const std::vector<std::string>& imagesPath)
{
	assert(imagesPath.size() == 6);

	// Create and bind the texture to the cubemap texture
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);

	// Loop over each image path
	for (int i = 0; i < 6; i++)
	{
		const std::string& currentImage = imagesPath[i];
		
		// Load the image using the stbi_image
		int x, y, n;
		unsigned char* imageData = stbi_load(currentImage.c_str(), &x, &y, &n, 0);
		
		if (!imageData)
		{
			std::cerr << "Error: cubemap texture image is not supported: " << currentImage << std::endl;
			stbi_image_free(imageData);
			assert(false);
		}

		// Attach texture data
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		
		stbi_image_free(imageData);
	}	

	// Filtering methods
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

Cubemap::~Cubemap()
{
	glDeleteTextures(1, &m_Id);
}

void Cubemap::BindTexture()
{
	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);
}

void Cubemap::LoadTextures(const std::vector<std::string>& imagesPath)
{
	assert(imagesPath.size() == 6);

	glBindTexture(GL_TEXTURE_CUBE_MAP, m_Id);

	int imageCount = 0;
	for (const std::string& imagePath : imagesPath)
	{
		int x, y, n;
		unsigned char* imageData = stbi_load(imagePath.c_str(), &x, &y, &n, 0);

		if (!imageData)
		{
			std::cerr << "Error: cubemap texture image is not supported: " << imagePath << std::endl;
			stbi_image_free(imageData);
			assert(false);
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + imageCount, 0, GL_RGB, x, y, 0, GL_RGB, GL_UNSIGNED_BYTE, imageData);
		stbi_image_free(imageData);
		imageCount++;
	}

	// Filtering methods
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}

#include "texture.h"
#include <glad/glad.h>

Texture::Texture(uint32_t width, uint32_t height, uint32_t intFormat, uint32_t sourceFormat, uint32_t dataType, const unsigned char* data)
{
	glGenTextures(1, &m_Id);
	glBindTexture(GL_TEXTURE_2D, m_Id);

	// Set the parameters
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexImage2D(GL_TEXTURE_2D, 0, intFormat, width, height, 0, sourceFormat, dataType, data);
	glGenerateMipmap(GL_TEXTURE_2D);
}

void Texture::Bind(int textureUnit) const
{
	glActiveTexture(GL_TEXTURE0 + textureUnit);
	glBindTexture(GL_TEXTURE_2D, m_Id);
}

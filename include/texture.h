#ifndef TEXTURE_H
#define TEXTURE_H

#include <cstdint>

class Texture
{
public:
	// TODO: create a regular stb_image handling constructor
	Texture(uint32_t width,
		uint32_t height,
		uint32_t intFormat,
		uint32_t sourceFormat,
		uint32_t dataType,
		const unsigned char* data);

	void Bind(int textureUnit) const;

private:
	unsigned int m_Id;
};

#endif
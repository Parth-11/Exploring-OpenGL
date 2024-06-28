#pragma once

#include <string>
#include <GL\glew.h>
#include <stb_image\stb_image.h>

class Texture {
private:
	unsigned int textureID;
	int width, height, bitDepth;

	std::string fileLocation;
public:
	Texture();
	Texture(const std::string &fileLocation);

	void LoadTexture();
	void UseTexture();
	void ClearTexture();

	~Texture();
};
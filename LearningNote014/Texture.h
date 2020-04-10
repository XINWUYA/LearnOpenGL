#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "stb_image.h"

class CTexture
{
public:
	CTexture(GLenum vTextureTarget, const std::string& vFilePath);
	CTexture(GLenum vTextureTarget, const std::vector<std::string>& vCubeFacePathSet);
	~CTexture();

	void bindImageTexture(GLenum vTextureUnit);

private:
	bool __loadImage(const std::string& vFilePath);
	bool __loadCubeMap(const std::vector<std::string>& vCubeFacesPathSet);
	GLenum m_TextureTarget;
	GLuint m_TextureObj;
};
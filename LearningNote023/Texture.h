#pragma once
#include <iostream>
#include <string>
#include <GLFW/glfw3.h>

class CTexture
{
public:
	CTexture(GLenum vTextureTarget, const std::string& vFilePath);
	~CTexture();

	void bindImageTexture(GLenum vTextureUnit);

private:
	bool __loadImage();
	std::string m_FilePath;
	GLenum m_TextureTarget;
	GLuint m_TextureObj;
	unsigned char* m_pTextureData;
	int m_TextureWidth;
	int m_TextureHeight;
	int m_NumChannels;
};
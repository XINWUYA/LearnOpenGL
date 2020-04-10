#include <GL/glew.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include "Texture.h"

CTexture::CTexture(GLenum vTextureTarget, const std::string & vFilePath)
{
	m_TextureTarget = vTextureTarget;
	m_FilePath = vFilePath;
	__loadImage();
}

CTexture::~CTexture()
{
	stbi_image_free(m_pTextureData);
}

//**********************************************************************************
//FUNCTION:
void CTexture::bindImageTexture(GLenum vTextureUnit)
{
	glActiveTexture(vTextureUnit);
	glEnable(m_TextureTarget);
	glBindTexture(m_TextureTarget, m_TextureObj);
}

//**********************************************************************************
//FUNCTION:
bool CTexture::__loadImage()
{
	glGenTextures(1, &m_TextureObj);
	glBindTexture(m_TextureTarget, m_TextureObj);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	m_pTextureData = stbi_load(m_FilePath.c_str(), &m_TextureWidth, &m_TextureHeight, &m_NumChannels, 0);

	if (m_pTextureData)
	{
		glTexImage2D(m_TextureTarget, 0, GL_RGB, m_TextureWidth, m_TextureHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, m_pTextureData);
		glGenerateMipmap(m_TextureTarget);
	}
	else
	{
		std::cout << "Failed to load texture" << std::endl;
		return false;
	}
	return true;
}

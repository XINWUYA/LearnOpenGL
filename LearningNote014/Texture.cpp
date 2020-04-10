#include "Texture.h"

CTexture::CTexture(GLenum vTextureTarget, const std::string & vFilePath)
{
	m_TextureTarget = vTextureTarget;
	__loadImage(vFilePath);
}

CTexture::CTexture(GLenum vTextureTarget, const std::vector<std::string>& vCubeFacesPathSet)
{
	m_TextureTarget = vTextureTarget;
	__loadCubeMap(vCubeFacesPathSet);
}

CTexture::~CTexture()
{
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
bool CTexture::__loadImage(const std::string& vFilePath)
{
	glGenTextures(1, &m_TextureObj);
	glBindTexture(m_TextureTarget, m_TextureObj);
	
	int TextureWidth, TextureHeight, TextureChannelNum;
	unsigned char* pTextureData = stbi_load(vFilePath.c_str(), &TextureWidth, &TextureHeight, &TextureChannelNum, 0);

	if (pTextureData)
	{
		GLenum TextureFormat;
		if      (1 == TextureChannelNum) TextureFormat = GL_RED;
		else if (3 == TextureChannelNum) TextureFormat = GL_RGB;
		else if (4 == TextureChannelNum) TextureFormat = GL_RGBA;

		glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_T, GL_REPEAT);
		if (4 == TextureChannelNum)
		{
			glTexParameteri(m_TextureTarget, GL_TEXTURE_MIN_FILTER, GL_CLAMP_TO_EDGE);
			glTexParameteri(m_TextureTarget, GL_TEXTURE_MAG_FILTER, GL_CLAMP_TO_EDGE);
		}
		else
		{
			glTexParameteri(m_TextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(m_TextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}

		glTexImage2D(m_TextureTarget, 0, TextureFormat, TextureWidth, TextureHeight, 0, TextureFormat, GL_UNSIGNED_BYTE, pTextureData);
		glGenerateMipmap(m_TextureTarget);
		stbi_image_free(pTextureData);
	}
	else
	{
		std::cout << "Error: Failed to load texture" << std::endl;
		return false;
	}
	return true;
}

//**********************************************************************************
//FUNCTION:
bool CTexture::__loadCubeMap(const std::vector<std::string>& vCubeFacesPathSet)
{
	glGenTextures(1, &m_TextureObj);
	glBindTexture(m_TextureTarget, m_TextureObj);

	int TextureWidth, TextureHeight, TextureChannelNum;
	for (auto i = 0; i < vCubeFacesPathSet.size(); i++)
	{
		unsigned char* pTextureData = stbi_load(vCubeFacesPathSet[i].c_str(), &TextureWidth, &TextureHeight, &TextureChannelNum, 0);
		if (pTextureData)
		{
			GLenum TextureFormat;
			if      (1 == TextureChannelNum) TextureFormat = GL_RED;
			else if (3 == TextureChannelNum) TextureFormat = GL_RGB;
			else if (4 == TextureChannelNum) TextureFormat = GL_RGBA;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, TextureFormat, TextureWidth, TextureHeight, 0, TextureFormat, GL_UNSIGNED_BYTE, pTextureData);
			stbi_image_free(pTextureData);
		}
		else
		{
			std::cout << "Error: Failed to load cubemap texture" << std::endl;
			return false;
		}
	}
	glTexParameteri(m_TextureTarget, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(m_TextureTarget, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	return true;
}

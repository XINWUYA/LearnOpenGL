#pragma once
#include <iostream>
#include <string>
#include <vector>

#include <GLM/glm.hpp>

#include "Shader.h"

struct SVertex
{
	glm::vec3 m_Position;
	glm::vec3 m_Normal;
	glm::vec2 m_TexCoords;
	glm::vec3 m_Tangent;//切线
	glm::vec3 m_Bitangent;//二重切线
};

struct STexture
{
	unsigned int m_TextureID;
	std::string m_TextureType;
	std::string m_TexturePath;
};

class CMesh
{
public:
	CMesh(const std::vector<SVertex>& vVerticesSet, const std::vector<unsigned int>& vIndicesSet, const std::vector<STexture>& vTexturesSet);
	~CMesh();

	void drawMesh(CShader* vShader, unsigned int vTextureUnit);
	unsigned int getVAO() const;
	std::vector<unsigned int> getIndicesSet() const;

private:
	void __setupMesh();

	unsigned int m_VAO, m_VBO, m_EBO;
	std::vector<SVertex> m_VerticesSet;
	std::vector<unsigned int> m_IndicesSet;
	std::vector<STexture> m_TexturesSet;
};
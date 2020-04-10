#include "Mesh.h"

CMesh::CMesh(const std::vector<SVertex>& vVerticesSet, const std::vector<unsigned int>& vIndicesSet, const std::vector<STexture>& vTexturesSet)
{
	m_VerticesSet = vVerticesSet;
	m_IndicesSet = vIndicesSet;
	m_TexturesSet = vTexturesSet;

	__setupMesh();
}

CMesh::~CMesh()
{
}

//**********************************************************************************
//FUNCTION:
void CMesh::drawMesh(CShader* vShader, unsigned int vTextureUnit)
{
	unsigned int DiffuseNum = 0;
	unsigned int SpecularNum = 0;
	unsigned int NormalNum = 0;
	unsigned int AmbientNum = 0;
	for (auto i = vTextureUnit; i < m_TexturesSet.size() + vTextureUnit; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, m_TexturesSet[i - vTextureUnit].m_TextureID);
		std::string TextureNum;
		std::string TextureName = m_TexturesSet[i - vTextureUnit].m_TextureType;
		if (TextureName == "m_Diffuse")
			TextureNum = std::to_string(DiffuseNum++);
		else if (TextureName == "m_Specular")
			TextureNum = std::to_string(SpecularNum++);
		else if (TextureName == "m_Normal")
			TextureNum = std::to_string(NormalNum++);
		else if (TextureName == "m_Ambient")
			TextureNum = std::to_string(AmbientNum++);

		vShader->setInt(("Material." + TextureName + TextureNum).c_str(), i);
		//glUniform1i(glGetUniformLocation(vShader.getShaderProgram(),("Material." + TextureName + TextureNum).c_str()), i);
	}

	glBindVertexArray(m_VAO);
	glDrawElements(GL_TRIANGLES, m_IndicesSet.size(), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	for (unsigned int i = vTextureUnit; i < m_TexturesSet.size() + vTextureUnit; i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		glBindTexture(GL_TEXTURE_2D, 0);
	}
}

//**********************************************************************************
//FUNCTION:
void CMesh::__setupMesh()
{
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);
	glGenBuffers(1, &m_EBO);

	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, m_VerticesSet.size() * sizeof(SVertex), &m_VerticesSet[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_IndicesSet.size() * sizeof(unsigned int), &m_IndicesSet[0], GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, m_Normal));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(SVertex), (void*)offsetof(SVertex, m_TexCoords));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
}

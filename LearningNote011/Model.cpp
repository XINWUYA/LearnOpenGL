#include "Model.h"

CModel::CModel(const std::string & vFilePath, bool vGammaCorrection) : m_GammaCorrection(vGammaCorrection)
{
	_ASSERT(vFilePath != "");
	__loadModel(vFilePath);
}

CModel::~CModel()
{
}

//**********************************************************************************
//FUNCTION:
void CModel::drawModel(CShader* vShader, unsigned int vTextureUnit)
{
	for (auto& Mesh : m_MeshSet)
		Mesh.drawMesh(vShader, vTextureUnit);
}

//**********************************************************************************
//FUNCTION:
void CModel::__loadModel(const std::string & vFilePath)
{
	_ASSERT(vFilePath != "");
	Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile(vFilePath, aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!pScene || pScene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !pScene->mRootNode)
	{
		std::cout << "Error: Assimp: " << Importer.GetErrorString() << std::endl;
		return;
	}
	m_Directory = vFilePath.substr(0, vFilePath.find_last_of('/'));
	__processNodeRecursively(pScene->mRootNode, pScene);
}

//**********************************************************************************
//FUNCTION:
void CModel::__processNodeRecursively(aiNode * vNode, const aiScene * vScene)
{
	_ASSERT(vNode && vScene);

	for (unsigned int i = 0; i < vNode->mNumMeshes; i++)
	{
		aiMesh* pMesh = vScene->mMeshes[vNode->mMeshes[i]];
		_ASSERT(pMesh);
		m_MeshSet.push_back(__processMesh(pMesh, vScene));
	}
	for (unsigned int i = 0; i < vNode->mNumChildren; i++)
	{
		__processNodeRecursively(vNode->mChildren[i], vScene);
	}
}

CMesh CModel::__processMesh(aiMesh * vMesh, const aiScene * vScene)
{
	_ASSERT(vMesh && vScene);

	std::vector<SVertex> VerticesSet;
	std::vector<unsigned int> IndicesSet;
	std::vector<STexture> TexturesSet;
	//Process Vertices
	for (unsigned int i = 0; i < vMesh->mNumVertices; i++)
	{
		SVertex Vertex;
		Vertex.m_Position = glm::vec3(vMesh->mVertices[i].x, vMesh->mVertices[i].y, vMesh->mVertices[i].z);
		Vertex.m_Normal = glm::vec3(vMesh->mNormals[i].x, vMesh->mNormals[i].y, vMesh->mNormals[i].z);
		
		if (vMesh->mTextureCoords[0])
			Vertex.m_TexCoords= glm::vec2(vMesh->mTextureCoords[0][i].x, vMesh->mTextureCoords[0][i].y);
		else
			Vertex.m_TexCoords = glm::vec2(0.0f, 0.0f);
		if(vMesh->mTangents)
			Vertex.m_Tangent = glm::vec3(vMesh->mTangents[i].x, vMesh->mTangents[i].y, vMesh->mTangents[i].z);
		if(vMesh->mBitangents)
			Vertex.m_Bitangent = glm::vec3(vMesh->mBitangents[i].x, vMesh->mBitangents[i].y, vMesh->mBitangents[i].z);

		VerticesSet.push_back(Vertex);
	}
	//Process Indices
	for (unsigned int i = 0; i < vMesh->mNumFaces; i++)
	{
		aiFace Face = vMesh->mFaces[i];
		for (unsigned int k = 0; k < Face.mNumIndices; k++)
			IndicesSet.push_back(Face.mIndices[k]);
	}
	//Process MAterials
	aiMaterial* pMaterial = vScene->mMaterials[vMesh->mMaterialIndex];
	_ASSERT(pMaterial);
	std::vector<STexture> DiffuseMaps = __loadMaterialTextures(pMaterial, aiTextureType_DIFFUSE, "m_Diffuse");
	TexturesSet.insert(TexturesSet.end(), DiffuseMaps.begin(), DiffuseMaps.end());
	std::vector<STexture> SpecularMaps = __loadMaterialTextures(pMaterial, aiTextureType_SPECULAR, "m_Specular");
	TexturesSet.insert(TexturesSet.end(), SpecularMaps.begin(), SpecularMaps.end());
	std::vector<STexture> NormalMaps = __loadMaterialTextures(pMaterial, aiTextureType_NORMALS, "m_Normal");
	TexturesSet.insert(TexturesSet.end(), NormalMaps.begin(), NormalMaps.end());
	std::vector<STexture> AmbientMaps = __loadMaterialTextures(pMaterial, aiTextureType_AMBIENT, "m_Ambient");
	TexturesSet.insert(TexturesSet.end(), AmbientMaps.begin(), AmbientMaps.end());
	
	return CMesh(VerticesSet, IndicesSet, TexturesSet);
}

//**********************************************************************************
//FUNCTION:
std::vector<STexture> CModel::__loadMaterialTextures(aiMaterial * vMaterial, aiTextureType vType, std::string vTypeName)
{
	_ASSERT(vMaterial && (vTypeName != ""));
	std::vector<STexture> TexturesSet;
	for (unsigned int i = 0; i < vMaterial->GetTextureCount(vType); i++)
	{
		aiString TexturePath;
		vMaterial->GetTexture(vType, i, &TexturePath);
		bool SkipFlag = false;
		for (unsigned int k = 0; k < m_TexturesLoadedSet.size(); k++)
		{
			if (std::strcmp(m_TexturesLoadedSet[k].m_TexturePath.data(), TexturePath.C_Str()) == 0)
			{
				TexturesSet.push_back(m_TexturesLoadedSet[k]);
				SkipFlag = true;
				break;
			}
		}
		if (!SkipFlag)
		{
			STexture Texture;
			Texture.m_TextureID = __generateTextureFromFile(TexturePath.C_Str(), m_Directory, m_GammaCorrection);
			Texture.m_TextureType = vTypeName;
			Texture.m_TexturePath = TexturePath.C_Str();
			TexturesSet.push_back(Texture);
			m_TexturesLoadedSet.push_back(Texture);
		}

	}
	return TexturesSet;
}

unsigned int CModel::__generateTextureFromFile(const std::string & vFilePath, const std::string & vDirectory, bool vGammaCorrection)
{
	_ASSERT((vFilePath != "") && (vDirectory != ""));
	
	std::string FileName = vDirectory + '/' + vFilePath;
	
	unsigned int TextureID;
	glGenTextures(1, &TextureID);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	int Width, Height, NumChannels;
	unsigned char* pData = stbi_load(FileName.c_str(), &Width, &Height, &NumChannels, 0);
	if (pData)
	{
		GLenum TextureFormat;
		if      (1 == NumChannels) TextureFormat = GL_RED;
		else if (3 == NumChannels) TextureFormat = GL_RGB;
		else if (4 == NumChannels) TextureFormat = GL_RGBA;

		glBindTexture(GL_TEXTURE_2D, TextureID);
		glTexImage2D(GL_TEXTURE_2D, 0, TextureFormat, Width, Height, 0, TextureFormat, GL_UNSIGNED_BYTE, pData);
		glGenerateMipmap(GL_TEXTURE_2D);

		stbi_image_free(pData);
	}
	else
	{
		std::cout << "Failed to load texture" << FileName << std::endl;
		TextureID = -1;
		stbi_image_free(pData);
	}
	return TextureID;
}

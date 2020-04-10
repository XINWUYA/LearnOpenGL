#pragma once
#include <iostream>
#include <string>

#include <Assimp/Importer.hpp>
#include <Assimp/scene.h>
#include <Assimp/postprocess.h>

#include "Shader.h"
#include "Mesh.h"
#include "stb_image.h"

class CModel
{
public:
	CModel(const std::string& vFilePath, bool vGammaCorrection = false);
	~CModel();

	void drawModel(CShader* vShader, unsigned int vTextureUnit);
	std::vector<CMesh> getMeshSet() const;
	std::vector<STexture> getTextureLoadedSet() const;

private:
	void __loadModel(const std::string& vFilePath);
	void __processNodeRecursively(aiNode* vNode, const aiScene* vScene);
	CMesh __processMesh(aiMesh* vMesh, const aiScene* vScene);
	std::vector<STexture> __loadMaterialTextures(aiMaterial* vMaterial, aiTextureType vType, std::string vTypeName);
	unsigned int __generateTextureFromFile(const std::string& vFilePath, const std::string& vDirectory, bool vGammaCorrection);
	
	std::vector<CMesh> m_MeshSet;
	std::vector<STexture> m_TexturesLoadedSet;
	std::string m_Directory;//目录
	bool m_GammaCorrection;
};
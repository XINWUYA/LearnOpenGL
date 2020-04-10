#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include "GraphicsCommon.h"

class CShader;
class CTexture;
class CCamera;
class CModel;

class CGraphicsApp
{
public:
	CGraphicsApp(int vWindowWidth, int vWindowHeight, std::string& vWindowName);
	~CGraphicsApp();

	void init();	
	void setCursorStatus(const ECursorMode& vCursorMode);
	void run();

private:
	bool __initGLFWWindow(int vWindowWidth, int vWindowHeight, std::string& vWindowName);
	void __initShader();
	void __initTexture();
	void __initVAO();
	void __initModel();
	void __initCallback();
	void __initGBuffer();

	GLFWwindow* m_pGLFWWindow = nullptr;
	std::shared_ptr<CShader> m_pShader = nullptr;
	std::shared_ptr<CShader> m_pLightShader = nullptr;
	std::shared_ptr<CShader> m_pGenerateSceneGBufferShader = nullptr;
	std::shared_ptr<CTexture> m_pAlbedoTexture = nullptr;
	std::shared_ptr<CTexture> m_pNormalTexture = nullptr;
	std::shared_ptr<CModel> m_pNanosuitModel = nullptr;
	int m_WindowWidth = 0, m_WindowHeight = 0;
	unsigned int m_QuadVAO = 0;
	unsigned int m_CubeVAO = 0;
	unsigned int m_GBuffer = 0;
	unsigned int m_PositionTex = 0;
	unsigned int m_NormalTex = 0;
	unsigned int m_AlbedoSpecTex = 0;
	glm::vec3 m_LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
	std::vector<glm::vec3> m_ModelPositionSet{};
	std::vector<glm::vec3> m_LightPositionSet{};
	std::vector<glm::vec3> m_LightColorSet{};
};
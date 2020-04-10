#pragma once
#include <iostream>
#include <string>
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include "GraphicsCommon.h"

class CShader;
class CTexture;
class CCamera;

class CGraphicsApp
{
public:
	CGraphicsApp(int vWindowWidth, int vWindowHeight, std::string& vWindowName);
	~CGraphicsApp();

	void init();	
	void setCursorStatus(const ECursorMode& vCursorMode);
	void openDepthTest();
	void run();

private:
	bool __initGLFWWindow(int vWindowWidth, int vWindowHeight, std::string& vWindowName);
	void __initShader();
	void __initTexture();
	void __initVAO();
	void __initFBO();
	void __initCallback();

	GLFWwindow* m_pGLFWWindow = nullptr;
	std::shared_ptr<CShader> m_pShader = nullptr;
	std::shared_ptr<CShader> m_pQuadShader = nullptr;
	std::shared_ptr<CShader> m_pCubeShader = nullptr;
	std::shared_ptr<CShader> m_pLightShader = nullptr;
	std::shared_ptr<CShader> m_pDrawShadowMapShader = nullptr;
	std::shared_ptr<CShader> m_pComputeShadowMapShader = nullptr;
	std::shared_ptr<CTexture> m_pTexture = nullptr;
	int m_WindowWidth = 0, m_WindowHeight = 0;
	unsigned int m_QuadVAO = 0;
	unsigned int m_CubeVAO = 0;
	unsigned int m_ShadowMapVAO = 0;
	unsigned int m_DrawShadowMapFBO;
	unsigned int m_ShadowMapTex;
	glm::vec3 m_LightPos = glm::vec3(-2.0f, 4.0f, -1.0f);
};
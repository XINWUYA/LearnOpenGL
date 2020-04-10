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
	void __initCallback();

	GLFWwindow* m_pGLFWWindow = nullptr;
	std::shared_ptr<CShader> m_pShader = nullptr;
	std::shared_ptr<CTexture> m_pAlbedoTexture = nullptr;
	std::shared_ptr<CTexture> m_pNormalTexture = nullptr;
	int m_WindowWidth = 0, m_WindowHeight = 0;
	unsigned int m_QuadVAO = 0;
	glm::vec3 m_LightPos = glm::vec3(0.5f, 1.0f, 0.3f);
};
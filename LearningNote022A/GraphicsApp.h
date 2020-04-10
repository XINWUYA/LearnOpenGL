#pragma once
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
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
	void openDepthTest();
	void run();

private:
	bool __initGLFWWindow(int vWindowWidth, int vWindowHeight, std::string& vWindowName);
	void __initShader();
	void __initTexture();
	void __initVAO();
	void __initCallback();

	GLFWwindow* m_pGLFWWindow = nullptr;
	std::shared_ptr<CShader> m_pShader = nullptr;
	std::shared_ptr<CShader> m_pLightShader = nullptr;
	std::shared_ptr<CTexture> m_pTexture = nullptr;
	std::shared_ptr<CTexture> m_pTexture1 = nullptr;
	int m_WindowWidth, m_WindowHeight;
	unsigned int m_VAO;
	unsigned int m_QuadVAO;
	unsigned int m_LightVAO;
};
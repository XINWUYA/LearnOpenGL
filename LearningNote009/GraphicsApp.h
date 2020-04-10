#pragma once
#include <iostream>
#include <string>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "GraphicsCommon.h"

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
	void __processInput(GLFWwindow *vWindow);

	GLFWwindow* m_pGLFWWindow;
	CShader* m_pShader;
	CShader* m_pLightShader;
	CTexture* m_pTexture;
	CTexture* m_pTexture1;
	int m_WindowWidth, m_WindowHeight;
	unsigned int m_VAO;
	unsigned int m_LightVAO;
	glm::vec3 m_CubePosition[10] = {
		glm::vec3( 0.0f,  0.0f,  0.0f),
		glm::vec3( 2.0f,  5.0f, -15.0f),
		glm::vec3(-1.5f, -2.2f, -2.5f),
		glm::vec3(-3.8f, -2.0f, -12.3f),
		glm::vec3( 2.4f, -0.4f, -3.5f),
		glm::vec3(-1.7f,  3.0f, -7.5f),
		glm::vec3( 1.3f, -2.0f, -2.5f),
		glm::vec3( 1.5f,  2.0f, -2.5f),
		glm::vec3( 1.5f,  0.2f, -1.5f),
		glm::vec3(-1.3f,  1.0f, -1.5f)
	};
	glm::vec3 m_LightPosition[4] = {
		glm::vec3( 0.7f,  0.2f,  2.0f),
		glm::vec3( 2.3f, -3.3f, -4.0f),
		glm::vec3(-4.5f,  2.0f, -12.0f),
		glm::vec3( 0.0f,  0.0f, -3.0f)
	};
};
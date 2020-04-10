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
#include "Model.h"

#define ROCK_AMOUNT 10000

enum ECursorMode
{
	NORMAL,
	HIDDEN,
	DISABLE
};

class CGraphicsApp
{
public:
	CGraphicsApp(int vWindowWidth, int vWindowHeight, std::string& vWindowName);
	~CGraphicsApp();

	void init();
	void setCursorStatus(const ECursorMode& vCursorMode);
	void openDepthTest();
	void run();

	void mouse_callback(GLFWwindow* vWindow, double vXPos, double vYPos);
	void scroll_callback(GLFWwindow* vWindow, double vXOffset, double vYOffset);

private:
	bool __initGLFWWindow(int vWindowWidth, int vWindowHeight, std::string& vWindowName);
	void __initShader();
	void __initTexture();
	void __importModel();
	void __initVAO();
	void __initCallback();
	void __processInput(GLFWwindow* vWindow);
	void __generateRockMatrices();

	GLFWwindow* m_pGLFWWindow;
	CShader* m_pShader;
	CShader* m_pRockShader;
	CTexture* m_pTexture;
	CTexture* m_pTexture1;
	CModel* m_pPlanetModel;
	CModel* m_pRockModel;
	int m_WindowWidth, m_WindowHeight;
	unsigned int m_VAO;
	unsigned int m_LightVAO;
	float m_LastX = 0.0;
	float m_LastY = 0.0;
	bool m_FirstMouse = true;
	float m_DeltaTime = 0.0f;
	float m_LastFrame = 0.0f;
	bool m_Polygon = false;
	glm::mat4 m_RockMatrices[ROCK_AMOUNT];
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
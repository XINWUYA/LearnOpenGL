#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#include "Shader.h"
#include "Camera.h"
#include <GLM/gtc/matrix_transform.hpp>

using namespace Xinwuya;

const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
float LastX = 800.0f / 2.0f;
float LastY = 600.0f / 2.0f;
bool FirstMouse = true;
float DeltaTime = 0.0f;
float LastFrame = 0.0f;
bool Polygon = false;

//**********************************************************************************
//FUNCTION:
void mouse_callback(GLFWwindow* vWindow, double vXPos, double vYPos)
{
	if (FirstMouse)
	{
		LastX = vXPos;
		LastY = vYPos;
		FirstMouse = false;
	}

	float XOffset = vXPos - LastX;
	float YOffset = LastY - vYPos;

	LastX = vXPos;
	LastY = vYPos;
	CCamera::getInstance()->processMouseMovement(XOffset, YOffset);
}

//**********************************************************************************
//FUNCTION:
void scroll_callback(GLFWwindow* vWindow, double vXOffset, double vYOffset)
{
	CCamera::getInstance()->processMouseScroll(vYOffset);
}

//**********************************************************************************
//FUNCTION:
void key_callback(GLFWwindow* vWindow, int vKey, int vScanCode, int vAction, int vMode)
{
	if (vKey == GLFW_KEY_P && vAction == GLFW_PRESS)
		Polygon = !Polygon;
}

//**********************************************************************************
//FUNCTION:
void processInput(GLFWwindow *vWindow)
{
	if (glfwGetKey(vWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(vWindow, true);

	if (glfwGetKey(vWindow, GLFW_KEY_W) == GLFW_PRESS)
		CCamera::getInstance()->processKeyBoard(ECameraMovement::FORWARD, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_S) == GLFW_PRESS)
		CCamera::getInstance()->processKeyBoard(ECameraMovement::BACKWARD, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_A) == GLFW_PRESS)
		CCamera::getInstance()->processKeyBoard(ECameraMovement::LEFT, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_D) == GLFW_PRESS)
		CCamera::getInstance()->processKeyBoard(ECameraMovement::RIGHT, DeltaTime);
}

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* pWindow = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "LoadHDRImage", nullptr, nullptr);
	if(!pWindow)
	{
		std::cout << "Error: Window Created Failed!" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);

	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK)
	{
		std::cout << "Error: GLEW Init Failed!" << std::endl;
		return -1;
	}

	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(pWindow, mouse_callback);
	glfwSetScrollCallback(pWindow, scroll_callback);
	glfwSetKeyCallback(pWindow, key_callback);
	
	glEnable(GL_DEPTH_TEST); 
	glDepthFunc(GL_LEQUAL);

	std::shared_ptr<CShader> pDrawHDRImageShader = std::make_shared<CShader>();
	pDrawHDRImageShader->addShader("Shaders/draw_hdr_image_vs.glsl", EShaderType::VERTEX_SHADER);
	pDrawHDRImageShader->addShader("Shaders/draw_hdr_image_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pDrawHDRImageShader->createShaderProgram();
	std::shared_ptr<CShader> pDrawCubeMapShader = std::make_shared<CShader>();
	pDrawCubeMapShader->addShader("Shaders/draw_cubemap_vs.glsl", EShaderType::VERTEX_SHADER);
	pDrawCubeMapShader->addShader("Shaders/draw_cubemap_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pDrawCubeMapShader->createShaderProgram();

	stbi_set_flip_vertically_on_load(true);
	int TextureWidth, TextureHeight, TextureChannels;
	float* pData = stbi_loadf("Textures/newport_loft.hdr", &TextureWidth, &TextureHeight, &TextureChannels, 0);
	unsigned int HDRTexture;
	if (pData)
	{
		glGenTextures(1, &HDRTexture);
		glBindTexture(GL_TEXTURE_2D, HDRTexture);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, TextureWidth, TextureHeight, 0, GL_RGB, GL_FLOAT, pData);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(pData);
	}
	else
		std::cout << "Error: Texture Loaded Failed!" << std::endl;

	float QuadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f, 0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f,  0.0f,

		-1.0f,  1.0f, 0.0f,  0.0f,  1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f,  1.0f
	};

	float CubeMapVertices[] = {
		// positions          
		-1.0f,  1.0f, -1.0f,
		-1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f, -1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,

		-1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,

		-1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f, -1.0f,
		 1.0f,  1.0f,  1.0f,
		 1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f,

		-1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f, -1.0f,
		 1.0f, -1.0f, -1.0f,
		-1.0f, -1.0f,  1.0f,
		 1.0f, -1.0f,  1.0f
	};

	unsigned int VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeMapVertices), CubeMapVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	_ASSERT(glGetError() == GL_NO_ERROR);

	unsigned int FBO, RBO;
	glGenFramebuffers(1, &FBO);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);

	glGenRenderbuffers(1, &RBO);
	glBindRenderbuffer(GL_RENDERBUFFER, RBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, RBO);

	unsigned int CubeMapTexture;
	glGenTextures(1, &CubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
	for(int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 512, 512, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	glm::mat4 Projection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
	glm::mat4 Views[] =
	{
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
		glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3( 0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
	};

	pDrawHDRImageShader->useShaderProgram();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, HDRTexture);
	pDrawHDRImageShader->setInt("u_EquirectangularMap", 0);
	pDrawHDRImageShader->setMat4("u_Projection", Projection);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, FBO);
	for(int i = 0; i < 6; ++i)
	{
		pDrawHDRImageShader->setMat4("u_View", Views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, CubeMapTexture, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	pDrawCubeMapShader->useShaderProgram();
	pDrawCubeMapShader->setInt("u_EnvironmentMap", 0);
	glm::mat4 CubeMapProjection = glm::perspective(glm::radians(CCamera::getInstance()->getCameraZoom()), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
	pDrawCubeMapShader->setMat4("u_Projection", CubeMapProjection);
	int ScrWidth, ScrHeight;
	glfwGetFramebufferSize(pWindow, &ScrWidth, &ScrHeight);
	glViewport(0, 0, ScrWidth, ScrHeight);

	while(!glfwWindowShouldClose(pWindow)  && !glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
	{
		float CurrentFrame = glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;
		processInput(pWindow);
		
		glClearColor(0.2, 0.3, 0.3, 1.0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		pDrawCubeMapShader->useShaderProgram();
		pDrawCubeMapShader->setMat4("u_View", CCamera::getInstance()->getViewMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glBindVertexArray(0);
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteFramebuffers(1, &VBO);
	glfwTerminate();
	return 0;
}
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
	//OpenGL Init
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

	//Input Callback
	glfwSetInputMode(pWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwSetCursorPosCallback(pWindow, mouse_callback);
	glfwSetScrollCallback(pWindow, scroll_callback);
	glfwSetKeyCallback(pWindow, key_callback);

	//Function Enable
	glEnable(GL_DEPTH_TEST); 
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	glDepthFunc(GL_LEQUAL);

	//Shaders
	std::shared_ptr<CShader> pDrawHDRImageShader = std::make_shared<CShader>();
	pDrawHDRImageShader->addShader("Shaders/draw_hdr_image_vs.glsl", EShaderType::VERTEX_SHADER);
	pDrawHDRImageShader->addShader("Shaders/draw_hdr_image_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pDrawHDRImageShader->createShaderProgram();
	std::shared_ptr<CShader> pDrawCubeMapShader = std::make_shared<CShader>();
	pDrawCubeMapShader->addShader("Shaders/draw_cubemap_vs.glsl", EShaderType::VERTEX_SHADER);
	pDrawCubeMapShader->addShader("Shaders/draw_cubemap_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pDrawCubeMapShader->createShaderProgram();
	std::shared_ptr<CShader> pSphereShader = std::make_shared<CShader>();
	pSphereShader->addShader("Shaders/draw_sphere_vs.glsl", EShaderType::VERTEX_SHADER);
	pSphereShader->addShader("Shaders/draw_sphere_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pSphereShader->createShaderProgram();
	std::shared_ptr<CShader> pPrecomputeIrradianceShader = std::make_shared<CShader>();
	pPrecomputeIrradianceShader->addShader("Shaders/draw_cubemap_vs.glsl", EShaderType::VERTEX_SHADER);
	pPrecomputeIrradianceShader->addShader("Shaders/precompute_irradiance_map_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pPrecomputeIrradianceShader->createShaderProgram();
	std::shared_ptr<CShader> pPrefilteredShader = std::make_shared<CShader>();
	pPrefilteredShader->addShader("Shaders/draw_cubemap_vs.glsl", EShaderType::VERTEX_SHADER);
	pPrefilteredShader->addShader("Shaders/prefilter_environment_map_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pPrefilteredShader->createShaderProgram();
	std::shared_ptr<CShader> pPrecomputeBRDFShader = std::make_shared<CShader>();
	pPrecomputeBRDFShader->addShader("Shaders/draw_quad_vs.glsl", EShaderType::VERTEX_SHADER);
	pPrecomputeBRDFShader->addShader("Shaders/precompute_brdf_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pPrecomputeBRDFShader->createShaderProgram();
	std::shared_ptr<CShader> pDrawQuadShader = std::make_shared<CShader>();
	pDrawQuadShader->addShader("Shaders/draw_quad_vs.glsl", EShaderType::VERTEX_SHADER);
	pDrawQuadShader->addShader("Shaders/draw_quad_fs.glsl", EShaderType::FRAGMENT_SHADER);
	pDrawQuadShader->createShaderProgram();

	// Some VAOs
	//矩形顶点及VAO
	float QuadVertices[] = {
		// positions   // texCoords
		-1.0f,  1.0f, 0.0f,  0.0f,  1.0f,
		-1.0f, -1.0f, 0.0f,  0.0f,  0.0f,
		 1.0f, -1.0f, 0.0f,  1.0f,  0.0f,

		-1.0f,  1.0f, 0.0f,  0.0f,  1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f,  0.0f,
		 1.0f,  1.0f, 0.0f,  1.0f,  1.0f
	};
	unsigned int QuadVAO, QuadVBO;
	glGenVertexArrays(1, &QuadVAO);
	glGenBuffers(1, &QuadVBO);
	glBindVertexArray(QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), QuadVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glBindVertexArray(0);

	//立方体顶点及VAO
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

	unsigned int CubeVAO, CubeVBO;
	glGenVertexArrays(1, &CubeVAO);
	glGenBuffers(1, &CubeVBO);
	glBindVertexArray(CubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeMapVertices), CubeMapVertices, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindVertexArray(0);

	_ASSERT(glGetError() == GL_NO_ERROR);


	//生成球面顶点、法线等及VAO
	std::vector<glm::vec3> SphereVerticesSet;
	std::vector<glm::vec2> SphereUVSet;
	std::vector<glm::vec3> SphereNormalsSet;
	std::vector<unsigned int> SphereIndicesSet;
	const unsigned int X_SEGMENTS = 64;
	const unsigned int Y_SEGMENTS = 64;
	const float PI = 3.14159265359;
	for(unsigned int y = 0; y <= Y_SEGMENTS; ++y)
	{
		for(unsigned int x = 0; x <= X_SEGMENTS; ++x)
		{
			float TempX = static_cast<float>(x) / static_cast<float>(X_SEGMENTS);
			float TempY = static_cast<float>(y) / static_cast<float>(Y_SEGMENTS);
			float TempPosX = std::cos(TempX * 2.0f * PI) * std::sin(TempY * PI);
			float TempPosY = std::cos(TempY * PI);
			float TempPosZ = std::sin(TempX * 2.0f * PI) * std::sin(TempY * PI);

			SphereVerticesSet.push_back(glm::vec3(TempPosX, TempPosY, TempPosZ));
			SphereUVSet.push_back(glm::vec2(TempX, TempY));
			SphereNormalsSet.push_back(glm::vec3(TempPosX, TempPosY, TempPosZ));
		}
	}
	bool OddRow = false;
	for(unsigned int y = 0; y < Y_SEGMENTS; ++y)
	{
		if(!OddRow)
		{
			for(unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				SphereIndicesSet.push_back(y * (X_SEGMENTS + 1) + x);
				SphereIndicesSet.push_back((y + 1) * (X_SEGMENTS + 1) + x);
			}
		}
		else
		{
			for(unsigned int x = 0; x <= X_SEGMENTS; ++x)
			{
				SphereIndicesSet.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				SphereIndicesSet.push_back(y * (X_SEGMENTS + 1) + x);
			}
		}
		OddRow = !OddRow;
	}
	unsigned int IndicesCnt = SphereIndicesSet.size();

	std::vector<float> SphereDataSet;
	for(unsigned int i = 0; i < SphereVerticesSet.size(); ++i)
	{
		SphereDataSet.push_back(SphereVerticesSet[i].x);
		SphereDataSet.push_back(SphereVerticesSet[i].y);
		SphereDataSet.push_back(SphereVerticesSet[i].z);
		if(!SphereUVSet.empty())
		{
			SphereDataSet.push_back(SphereUVSet[i].x);
			SphereDataSet.push_back(SphereUVSet[i].y);
		}
		if(!SphereNormalsSet.empty())
		{
			SphereDataSet.push_back(SphereNormalsSet[i].x);
			SphereDataSet.push_back(SphereNormalsSet[i].y);
			SphereDataSet.push_back(SphereNormalsSet[i].z);
		}
	}

	unsigned int SphereVAO, SphereVBO, SphereEBO;
	glGenVertexArrays(1, &SphereVAO);
	glGenBuffers(1, &SphereVBO);
	glGenBuffers(1, &SphereEBO);
	glBindVertexArray(SphereVAO);
	glBindBuffer(GL_ARRAY_BUFFER, SphereVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(float) * SphereDataSet.size(), &SphereDataSet[0], GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, SphereEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * SphereIndicesSet.size(), &SphereIndicesSet[0], GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindVertexArray(0);

	//FBO
	unsigned int CubeMapFBO, CubeMapRBO;
	glGenFramebuffers(1, &CubeMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);

	glGenRenderbuffers(1, &CubeMapRBO);
	glBindRenderbuffer(GL_RENDERBUFFER, CubeMapRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferRenderbuffer(GL_RENDERBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, CubeMapRBO);

	//Some Textures
	//Load HDR Texture
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

	//CubeMap
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

	glm::mat4 CubeMapProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
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
	pDrawHDRImageShader->setMat4("u_Projection", CubeMapProjection);

	glViewport(0, 0, 512, 512);
	glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);
	for(int i = 0; i < 6; ++i)
	{
		pDrawHDRImageShader->setMat4("u_View", Views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, CubeMapTexture, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//预计算场景辐射度
	unsigned int IrradianceMapTexture;
	glGenTextures(1, &IrradianceMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceMapTexture);
	for (int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 32, 32, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, CubeMapRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 32, 32);
	pPrecomputeIrradianceShader->useShaderProgram();
	pPrecomputeIrradianceShader->setInt("u_EnvironmentMap", 0);
	pPrecomputeIrradianceShader->setMat4("u_Projection", CubeMapProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
	glViewport(0, 0, 32, 32);
	glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);
	for (int i = 0; i < 6; ++i)
	{
		pPrecomputeIrradianceShader->setMat4("u_View", Views[i]);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, IrradianceMapTexture, 0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBindVertexArray(CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//预过滤环境贴图纹理
	unsigned int PrefilterCubeMapTexture;
	glGenTextures(1, &PrefilterCubeMapTexture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, PrefilterCubeMapTexture);
	for (int i = 0; i < 6; ++i)
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB16F, 128, 128, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

	pPrefilteredShader->useShaderProgram();
	pPrefilteredShader->setInt("u_EnvironmentMap", 0);
	pPrefilteredShader->setMat4("u_Projection", CubeMapProjection);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, CubeMapTexture);
	glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);
	unsigned int MaxMipMapLevel = 5;
	for (unsigned int i = 0; i < MaxMipMapLevel; ++i)
	{
		unsigned int TempMipWidth = 128 * std::pow(0.5, i);
		unsigned int TempMipHeight = 128 * std::pow(0.5, i);
		glBindRenderbuffer(GL_RENDERBUFFER, CubeMapRBO);
		glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, TempMipWidth, TempMipHeight);
		glViewport(0, 0, TempMipWidth, TempMipHeight);

		float Roughness = (float)i / (float)(MaxMipMapLevel - 1);
		pPrefilteredShader->setFloat("u_Roughness", Roughness);
		for (unsigned int k = 0; k < 6; ++k)
		{
			pPrefilteredShader->setMat4("u_View", Views[k]);
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_CUBE_MAP_POSITIVE_X + k, PrefilterCubeMapTexture, i);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
			glBindVertexArray(CubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//预计算BRDF查找纹理
	unsigned int BRDFTexture;
	glGenTextures(1, &BRDFTexture);
	glBindTexture(GL_TEXTURE_2D, BRDFTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 512, 512, 0, GL_RG, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBindFramebuffer(GL_FRAMEBUFFER, CubeMapFBO);
	glBindRenderbuffer(GL_RENDERBUFFER, CubeMapRBO);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT24, 512, 512);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, BRDFTexture, 0);
	glViewport(0, 0, 512, 512);
	pPrecomputeBRDFShader->useShaderProgram();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glBindVertexArray(QuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	//预设置一些着色器配置
	pDrawCubeMapShader->useShaderProgram();
	pDrawCubeMapShader->setInt("u_EnvironmentMap", 0);
	glm::mat4 Projection = glm::perspective(glm::radians(CCamera::getInstance()->getCameraZoom()), static_cast<float>(WINDOW_WIDTH) / static_cast<float>(WINDOW_HEIGHT), 0.1f, 100.0f);
	pDrawCubeMapShader->setMat4("u_Projection", Projection);

	glm::vec3 LightPositions[] = {
		glm::vec3(-10.0f,  10.0f, 10.0f),
		glm::vec3( 10.0f,  10.0f, 10.0f),
		glm::vec3(-10.0f, -10.0f, 10.0f),
		glm::vec3( 10.0f, -10.0f, 10.0f)
	};
	glm::vec3 LightColors[] = {
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f),
		glm::vec3(300.0f, 300.0f, 300.0f)
	};
	int RowNums = 7;
	int ColNums = 7;
	float Spacing = 2.5;
	pSphereShader->useShaderProgram();
	pSphereShader->setMat4("u_Projection", Projection);
	pSphereShader->setVec3("u_Albedo", 0.24f, 0.24f, 0.24f);
	pSphereShader->setFloat("u_AO", 1.0f);

	int ScrWidth, ScrHeight;
	glfwGetFramebufferSize(pWindow, &ScrWidth, &ScrHeight);
	glViewport(0, 0, ScrWidth, ScrHeight);

	//Main Loop
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
		glBindVertexArray(CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		pSphereShader->useShaderProgram();
		pSphereShader->setMat4("u_View", CCamera::getInstance()->getViewMatrix());
		pSphereShader->setVec3("u_CameraPos", CCamera::getInstance()->getCameraPosition());
		pSphereShader->setInt("u_IrradianceMap", 0);
		pSphereShader->setInt("u_PrefilteredMap", 1);
		pSphereShader->setInt("u_BRDFMap", 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_CUBE_MAP, IrradianceMapTexture);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_CUBE_MAP, PrefilterCubeMapTexture);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, BRDFTexture);

		for (unsigned int i = 0; i < sizeof(LightPositions) / sizeof(LightPositions[0]); ++i)
		{
			glm::vec3 TempLightPos = LightPositions[i] + glm::vec3(sin(glfwGetTime() * 5.0f) * 5.0f, 0.0f, 0.0f);
			//TempLightPos = LightPositions[i];
			pSphereShader->setVec3("u_LightPositionArray[" + std::to_string(i) + "]", TempLightPos);
			pSphereShader->setVec3("u_LightColorArray[" + std::to_string(i) + "]", LightColors[i]);
		}

		glm::mat4 Model;
		for (int i = 0; i < RowNums; ++i)
		{
			pSphereShader->setFloat("u_Metallic", static_cast<float>(i) / static_cast<float>(RowNums));
			for (int k = 0; k < ColNums; ++k)
			{
				pSphereShader->setFloat("u_Roughness", glm::clamp(static_cast<float>(k) / static_cast<float>(ColNums), 0.05f, 1.0f));
				Model = glm::mat4();
				Model = glm::translate(Model, glm::vec3(static_cast<float>(k - (ColNums / 2)) * Spacing, static_cast<float>(i - RowNums / 2) * Spacing, -2.0f));
				pSphereShader->setMat4("u_Model", Model);
				glBindVertexArray(SphereVAO);
				glDrawElements(GL_TRIANGLE_STRIP, SphereIndicesSet.size(), GL_UNSIGNED_INT, 0);
			}
		}

		//测试BRDF结果
		/*pDrawQuadShader->useShaderProgram();
		pDrawQuadShader->setInt("u_Texture", 0);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, BRDFTexture);
		glBindVertexArray(QuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);*/

		glBindVertexArray(0);
		glfwSwapBuffers(pWindow);
		glfwPollEvents();
	}

	glDeleteVertexArrays(1, &CubeVAO);
	glDeleteFramebuffers(1, &CubeVBO);
	glfwTerminate();
	return 0;
}
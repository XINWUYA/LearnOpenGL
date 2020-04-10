#include "GraphicsApp.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
#include "Model.h"
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>

CGraphicsApp::CGraphicsApp(int vWindowWidth, int vWindowHeight, std::string& vWindowName)
{
	__initGLFWWindow(vWindowWidth, vWindowHeight, vWindowName);
	_ASSERTE(glGetError() == GL_NO_ERROR);
	
	
	m_WindowWidth = vWindowWidth;
	m_WindowHeight = vWindowHeight;
}

CGraphicsApp::~CGraphicsApp()
{
	glDeleteVertexArrays(1, &m_QuadVAO);
	glfwTerminate();
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::init()
{
	__initCallback();
	__initShader();
	//__initTexture();
	__initVAO();
	__initModel();
	__initGBuffer();

	m_ModelPositionSet.emplace_back(glm::vec3(-3.0, -3.0, -3.0));
	m_ModelPositionSet.emplace_back(glm::vec3( 0.0, -3.0, -3.0));
	m_ModelPositionSet.emplace_back(glm::vec3( 3.0, -3.0, -3.0));
	m_ModelPositionSet.emplace_back(glm::vec3(-3.0, -3.0,  0.0));
	m_ModelPositionSet.emplace_back(glm::vec3( 0.0, -3.0,  0.0));
	m_ModelPositionSet.emplace_back(glm::vec3( 3.0, -3.0,  0.0));
	m_ModelPositionSet.emplace_back(glm::vec3(-3.0, -3.0,  3.0));
	m_ModelPositionSet.emplace_back(glm::vec3( 0.0, -3.0,  3.0));
	m_ModelPositionSet.emplace_back(glm::vec3( 3.0, -3.0,  3.0));

	const GLuint NUM_LIGHTS = 32;
	srand(13);
	for (auto i = 0; i < NUM_LIGHTS; ++i)
	{
		GLfloat XPos = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
		GLfloat YPos = ((rand() % 100) / 100.0f) * 6.0f - 4.0f;
		GLfloat ZPos = ((rand() % 100) / 100.0f) * 6.0f - 3.0f;
		m_LightPositionSet.emplace_back(glm::vec3(XPos, YPos, ZPos));

		GLfloat RColor = ((rand() % 100) / 200.0f) + 0.5f;
		GLfloat GColor = ((rand() % 100) / 200.0f) + 0.5f;
		GLfloat BColor = ((rand() % 100) / 200.0f) + 0.5f;
		m_LightColorSet.emplace_back(glm::vec3(RColor, GColor, BColor));
	}


	CCamera::get_mutable_instance().setCameraPosition(glm::vec3(0.0f, 3.0f, 5.0f));
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::setCursorStatus(const ECursorMode& vCursorMode)
{
	switch (vCursorMode)
	{
	case ECursorMode::NORMAL : glfwSetInputMode(m_pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); break;
	case ECursorMode::HIDDEN : glfwSetInputMode(m_pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); break;
	case ECursorMode::DISABLE: glfwSetInputMode(m_pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); break;
	default: break;
	}
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::run()
{
	while (!glfwWindowShouldClose(m_pGLFWWindow) && !glfwGetKey(m_pGLFWWindow, GLFW_KEY_ESCAPE))
	{
		GLfloat CurrentTime = glfwGetTime();
		DeltaTime = CurrentTime - LastTime;
		LastTime = CurrentTime;

		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		//glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); 
		
		processInput(m_pGLFWWindow);
		glEnable(GL_DEPTH_TEST);

		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(CCamera::get_mutable_instance().getCameraZoom()), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
		glm::mat4 ViewMatrix = CCamera::get_mutable_instance().getViewMatrix();
		glm::mat4 ModelMatrix = glm::mat4(1.0f);

		glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pGenerateSceneGBufferShader->useShaderProgram();
		m_pGenerateSceneGBufferShader->setMat4("uProjection", ProjectionMatrix);
		m_pGenerateSceneGBufferShader->setMat4("uView", ViewMatrix);
		for (int i = 0; i < (int)m_ModelPositionSet.size(); ++i)
		{
			ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, m_ModelPositionSet[i]);
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.25f));
			m_pGenerateSceneGBufferShader->setMat4("uModel", ModelMatrix);
			m_pNanosuitModel->drawModel(&(*m_pGenerateSceneGBufferShader), 0);
		}
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//Scene
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		m_pShader->useShaderProgram();
		m_pShader->setVec3("uCameraPos", CCamera::get_mutable_instance().getCameraPosition());
		m_pShader->setVec3("uLightPos", m_LightPos); 
		m_pShader->setInt("uPositionTex", 0);
		m_pShader->setInt("uNormalTex", 1);
		m_pShader->setInt("uAlbedoSpecTex", 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_PositionTex);
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, m_NormalTex);
		glActiveTexture(GL_TEXTURE2);
		glBindTexture(GL_TEXTURE_2D, m_AlbedoSpecTex);
		glBindVertexArray(m_QuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		glDisable(GL_DEPTH_TEST);

		m_pLightShader->useShaderProgram();
		m_pLightShader->setMat4("uProjection", ProjectionMatrix);
		m_pLightShader->setMat4("uView", ViewMatrix);
		for (int i = 0; i < (int)m_LightPositionSet.size(); ++i)
		{
			ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, m_LightPositionSet[i]);
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.25f));		
			m_pLightShader->setMat4("uModel", ModelMatrix);
			m_pLightShader->setVec3("uLightColor", m_LightColorSet[i]);
			glBindVertexArray(m_CubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}


		glfwPollEvents();
		glfwSwapBuffers(m_pGLFWWindow);
	}
}

//**********************************************************************************
//FUNCTION:
bool CGraphicsApp::__initGLFWWindow(int vWindowWidth, int vWindowHeight, std::string & vWindowName)
{
	_ASSERT(vWindowWidth && vWindowHeight);
	
	if (!glfwInit())
	{
		std::cout << "Error: GLFW init failed." << std::endl;
		return false;
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
	//glfwWindowHint(GLFW_SAMPLES, 64);
	glEnable(GL_MULTISAMPLE);
	m_pGLFWWindow = glfwCreateWindow(vWindowWidth, vWindowHeight, vWindowName.c_str(), nullptr, nullptr);
	if (m_pGLFWWindow == NULL)
	{
		std::cout << "Failed to Create GLFW Window." << std::endl;
		glfwTerminate();
		return false;
	}
	glfwMakeContextCurrent(m_pGLFWWindow);

	glewExperimental = GL_TRUE;
	if (glewInit() != GLEW_OK)
	{
		std::cout << "Error: GLEW init failed." << std::endl;
		return false;
	}
	_ASSERTE(glGetError() == GL_NO_ERROR);

	return true;
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initShader()
{
	m_pShader = std::make_shared<CShader>();
	m_pShader->addShader("../ShaderSources/LN023A/SceneVertShader.glsl", VERTEX_SHADER);
	m_pShader->addShader("../ShaderSources/LN023A/SceneFragShader.glsl", FRAGMENT_SHADER);
	m_pShader->createShaderProgram();

	m_pLightShader = std::make_shared<CShader>();
	m_pLightShader->addShader("../ShaderSources/LN023A/LightVertShader.glsl", VERTEX_SHADER);
	m_pLightShader->addShader("../ShaderSources/LN023A/LightFragShader.glsl", FRAGMENT_SHADER);
	m_pLightShader->createShaderProgram();

	m_pGenerateSceneGBufferShader = std::make_shared<CShader>();
	m_pGenerateSceneGBufferShader->addShader("../ShaderSources/LN023A/GenerateSceneGBufferVert.glsl", VERTEX_SHADER);
	m_pGenerateSceneGBufferShader->addShader("../ShaderSources/LN023A/GenerateSceneGBufferFrag.glsl", FRAGMENT_SHADER);
	m_pGenerateSceneGBufferShader->createShaderProgram();
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initTexture()
{
	m_pAlbedoTexture = std::make_shared<CTexture>(GL_TEXTURE_2D, "../TextureSources/brickwall.jpg");
	m_pNormalTexture = std::make_shared<CTexture>(GL_TEXTURE_2D, "../TextureSources/brickwall_normal.jpg");
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initVAO()
{
	float QuadVertices[] = {
		// Positions        // Normal         // Texture Coords
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		-1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
		 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		-1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,
		 1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,
		 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,
	};

	GLfloat vertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 1.0f, 1.0f,  // top-right
		-0.5f, -0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
		-0.5f, 0.5f, -0.5f, 0.0f, 0.0f, -1.0f, 0.0f, 1.0f,// top-left
		// Front face
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f,  // top-right
		0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-0.5f, 0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f,  // top-left
		-0.5f, -0.5f, 0.5f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
		// Left face
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		-0.5f, 0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-left
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-left
		-0.5f, -0.5f, -0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,  // bottom-right
		-0.5f, 0.5f, 0.5f, -1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
		// Right face
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-left
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top-right         
		0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,  // bottom-right
		0.5f, 0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,  // top-left
		0.5f, -0.5f, 0.5f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, // bottom-left     
		// Bottom face
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 1.0f, // top-left
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f,// bottom-left
		0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 1.0f, 0.0f, // bottom-left
		-0.5f, -0.5f, 0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // top-right
		// Top face
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // top-right     
		0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom-right
		-0.5f, 0.5f, -0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,// top-left
		-0.5f, 0.5f, 0.5f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f // bottom-left        
	};
	unsigned int CubeVBO;
	glGenVertexArrays(1, &m_CubeVAO);
	glGenBuffers(1, &CubeVBO);
	// Fill buffer
	glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
	// Link vertex attributes
	glBindVertexArray(m_CubeVAO);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int QuadVBO;
	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &QuadVBO);
	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), &QuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initModel()
{
	m_pNanosuitModel = std::make_shared<CModel>("../ModelSources/nanosuit/nanosuit.obj");
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initCallback()
{
	glfwSetCursorPosCallback(m_pGLFWWindow, mouse_callback);
	glfwSetScrollCallback(m_pGLFWWindow, scroll_callback);
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initGBuffer()
{
	glGenFramebuffers(1, &m_GBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, m_GBuffer);

	glGenTextures(1, &m_PositionTex);
	glBindTexture(GL_TEXTURE_2D, m_PositionTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_WindowWidth, m_WindowHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_PositionTex, 0);

	glGenTextures(1, &m_NormalTex);
	glBindTexture(GL_TEXTURE_2D, m_NormalTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, m_WindowWidth, m_WindowHeight, 0, GL_RGB, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, m_NormalTex, 0);

	glGenTextures(1, &m_AlbedoSpecTex);
	glBindTexture(GL_TEXTURE_2D, m_AlbedoSpecTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_WindowWidth, m_WindowHeight, 0, GL_RGBA, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, m_AlbedoSpecTex, 0);

	GLuint Attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, Attachments);

	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, m_WindowWidth, m_WindowHeight);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
	// - Finally check if framebuffer is complete
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
		std::cout << "Framebuffer not complete!" << std::endl;
	
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

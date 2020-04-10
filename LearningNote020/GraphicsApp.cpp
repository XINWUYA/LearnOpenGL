#include "GraphicsApp.h"
#include "Shader.h"
#include "Texture.h"
#include "Camera.h"
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
	__initTexture();
	__initVAO();
	__initFBO();
}

void CGraphicsApp::setCursorStatus(const ECursorMode& vCursorMode)
{
	switch (vCursorMode)
	{
	case NORMAL : glfwSetInputMode(m_pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL); break;
	case HIDDEN : glfwSetInputMode(m_pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN); break;
	case DISABLE: glfwSetInputMode(m_pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED); break;
	default: break;
	}
}

void CGraphicsApp::openDepthTest()
{
	glEnable(GL_DEPTH_TEST);
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

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		processInput(m_pGLFWWindow);
		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);

		glm::mat4 LightProjectionMat = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, 1.5f, 10.0f);
		glm::mat4 LightViewMat = glm::lookAt(m_LightPos, glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		glm::mat4 LightSpaceMatrix = LightProjectionMat * LightViewMat;
		m_pComputeShadowMapShader->useShaderProgram();
		m_pComputeShadowMapShader->setMat4("u_LightSpaceMat", LightSpaceMatrix);
		glViewport(0, 0, 1024, 1024);
		glBindFramebuffer(GL_FRAMEBUFFER, m_DrawShadowMapFBO);
		glCullFace(GL_FRONT);
		glClear(GL_DEPTH_BUFFER_BIT);
		{
			//Plane
			glm::mat4 ModelMatrix = glm::mat4(1.0f);
			m_pComputeShadowMapShader->setMat4("u_ModelMat", ModelMatrix);
			glBindVertexArray(m_QuadVAO);
			glDrawArrays(GL_TRIANGLES, 0, 6);
			glBindVertexArray(0);

			//Cube 1
			ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 1.5f, 0.0f));
			m_pComputeShadowMapShader->setMat4("u_ModelMat", ModelMatrix);
			glBindVertexArray(m_CubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			//Cube 2
			ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, 0.0f, 1.0f));
			m_pComputeShadowMapShader->setMat4("u_ModelMat", ModelMatrix);
			glBindVertexArray(m_CubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);

			//Cube 3
			ModelMatrix = glm::mat4(1.0f);
			ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.0f, 0.0f, 2.0f));
			ModelMatrix = glm::rotate(ModelMatrix, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
			ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f));
			m_pComputeShadowMapShader->setMat4("u_ModelMat", ModelMatrix);
			glBindVertexArray(m_CubeVAO);
			glDrawArrays(GL_TRIANGLES, 0, 36);
			glBindVertexArray(0);
		}
		glCullFace(GL_BACK);
		glBindFramebuffer(GL_FRAMEBUFFER, 0);

		//glDisable(GL_CULL_FACE);

		glViewport(0, 0, m_WindowWidth, m_WindowHeight);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/*m_pDrawShadowMapShader->useShaderProgram();
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(CCamera::get_mutable_instance().getCameraZoom()), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
		m_pDrawShadowMapShader->setMat4("uModel", ModelMatrix);
		m_pDrawShadowMapShader->setMat4("uProjection", ProjectionMatrix);
		m_pDrawShadowMapShader->setMat4("uView", CCamera::get_mutable_instance().getViewMatrix());
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMapTex);
		glBindVertexArray(m_ShadowMapVAO);
		glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);*/

		//Plane
		m_pQuadShader->useShaderProgram();
		glm::mat4 ModelMatrix = glm::mat4(1.0f);
		glm::mat4 ProjectionMatrix = glm::perspective(glm::radians(CCamera::get_mutable_instance().getCameraZoom()), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
		m_pQuadShader->setInt("uTexture", 0);
		m_pQuadShader->setMat4("uModel", ModelMatrix);
		m_pQuadShader->setMat4("uProjection", ProjectionMatrix);
		m_pQuadShader->setMat4("u_LightSpaceMat", LightSpaceMatrix);
		m_pQuadShader->setMat4("uView", CCamera::get_mutable_instance().getViewMatrix());
		m_pQuadShader->setVec3("uCameraPos", CCamera::get_mutable_instance().getCameraPosition());
		m_pQuadShader->setBool("uIsUseBlinn", true);
		m_pQuadShader->setInt("u_ShadowMapTex", m_ShadowMapTex);
		m_pTexture->bindImageTexture(0);
		glActiveTexture(GL_TEXTURE0 + m_ShadowMapTex);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMapTex);
		glBindVertexArray(m_QuadVAO);
		glDrawArrays(GL_TRIANGLES, 0, 6);

		//Cube 1
		m_pCubeShader->useShaderProgram();
		ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(0.0f, 1.5f, 0.0f));
		m_pCubeShader->setMat4("uModel", ModelMatrix);
		m_pCubeShader->setMat4("uProjection", ProjectionMatrix);
		m_pCubeShader->setMat4("u_LightSpaceMat", LightSpaceMatrix);
		m_pCubeShader->setMat4("uView", CCamera::get_mutable_instance().getViewMatrix());
		m_pCubeShader->setInt("u_ShadowMapTex", m_ShadowMapTex);
		glActiveTexture(GL_TEXTURE0 + m_ShadowMapTex);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMapTex); 
		glBindVertexArray(m_CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cube 2
		m_pCubeShader->useShaderProgram();
		ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(2.0f, 0.0f, 1.0f));
		m_pCubeShader->setMat4("uModel", ModelMatrix);
		m_pCubeShader->setMat4("uProjection", ProjectionMatrix);
		m_pCubeShader->setMat4("u_LightSpaceMat", LightSpaceMatrix);
		m_pCubeShader->setMat4("uView", CCamera::get_mutable_instance().getViewMatrix());
		m_pCubeShader->setInt("u_ShadowMapTex", m_ShadowMapTex);
		glActiveTexture(GL_TEXTURE0 + m_ShadowMapTex);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMapTex);
		glBindVertexArray(m_CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Cube 3
		m_pCubeShader->useShaderProgram();
		ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, glm::vec3(-1.0f, 0.0f, 2.0f));
		ModelMatrix = glm::rotate(ModelMatrix, glm::radians(60.0f), glm::normalize(glm::vec3(1.0f, 0.0f, 1.0f)));
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.5f));
		m_pCubeShader->setMat4("uModel", ModelMatrix);
		m_pCubeShader->setMat4("uProjection", ProjectionMatrix);
		m_pCubeShader->setMat4("u_LightSpaceMat", LightSpaceMatrix);
		m_pCubeShader->setMat4("uView", CCamera::get_mutable_instance().getViewMatrix());
		m_pCubeShader->setInt("u_ShadowMapTex", m_ShadowMapTex);
		glActiveTexture(GL_TEXTURE0 + m_ShadowMapTex);
		glBindTexture(GL_TEXTURE_2D, m_ShadowMapTex);
		glBindVertexArray(m_CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		//Light
		m_pLightShader->useShaderProgram();
		ModelMatrix = glm::mat4(1.0f);
		ModelMatrix = glm::translate(ModelMatrix, m_LightPos);
		ModelMatrix = glm::scale(ModelMatrix, glm::vec3(0.05f));
		m_pLightShader->setMat4("uModel", ModelMatrix);
		m_pLightShader->setMat4("uProjection", ProjectionMatrix);
		m_pLightShader->setMat4("u_LightSpaceMat", LightSpaceMatrix);
		m_pLightShader->setMat4("uView", CCamera::get_mutable_instance().getViewMatrix());
		glBindVertexArray(m_CubeVAO);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		glDisable(GL_DEPTH_TEST);

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
	m_pQuadShader = std::make_shared<CShader>();
	m_pQuadShader->addShader("../ShaderSources/LN020/QuadVertShader.glsl", VERTEX_SHADER);
	m_pQuadShader->addShader("../ShaderSources/LN020/QuadFragShader.glsl", FRAGMENT_SHADER);
	m_pQuadShader->createShaderProgram();

	m_pCubeShader = std::make_shared<CShader>();
	m_pCubeShader->addShader("../ShaderSources/LN020/CubeVertShader.glsl", VERTEX_SHADER);
	m_pCubeShader->addShader("../ShaderSources/LN020/CubeFragShader.glsl", FRAGMENT_SHADER);
	m_pCubeShader->createShaderProgram();

	m_pLightShader = std::make_shared<CShader>();
	m_pLightShader->addShader("../ShaderSources/LN020/LightVertShader.glsl", VERTEX_SHADER);
	m_pLightShader->addShader("../ShaderSources/LN020/LightFragShader.glsl", FRAGMENT_SHADER);
	m_pLightShader->createShaderProgram();

	m_pComputeShadowMapShader = std::make_shared<CShader>();
	m_pComputeShadowMapShader->addShader("../ShaderSources/LN020/ComputeShadowMapVertShader.glsl", VERTEX_SHADER);
	m_pComputeShadowMapShader->addShader("../ShaderSources/LN020/ComputeShadowMapFragShader.glsl", FRAGMENT_SHADER);
	m_pComputeShadowMapShader->createShaderProgram();

	m_pDrawShadowMapShader = std::make_shared<CShader>();
	m_pDrawShadowMapShader->addShader("../ShaderSources/LN020/DrawShadowMapVertShader.glsl", VERTEX_SHADER);
	m_pDrawShadowMapShader->addShader("../ShaderSources/LN020/DrawShadowMapFragShader.glsl", FRAGMENT_SHADER);
	m_pDrawShadowMapShader->createShaderProgram();
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initTexture()
{
	m_pTexture = std::make_shared<CTexture>(GL_TEXTURE_2D, "../TextureSources/wall.jpg");
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initVAO()
{
	float QuadVertices[] = {
		// Positions        // Texture Coords
        -1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
        -1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
         1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
         1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	};
	float PlaneVertices[] = {
		// positions            // normals         // texcoords
		 25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  25.0f,  0.0f,
		-25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  25.0f,
		-25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  0.0f,   0.0f,
							    	   		  		 	    
		 25.0f, -0.5f,  25.0f,  0.0f,  1.0f,  0.0f,  25.0f,  0.0f,
		 25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  25.0f, 25.0f,
		-25.0f, -0.5f, -25.0f,  0.0f,  1.0f,  0.0f,  0.0f,  25.0f
	};
	float CubeVertices[] = {
		// Back face
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f, // Bottom-left
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 0.0f, // bottom-right         
		 0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 1.0f, 1.0f,  // top-right
		-0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 0.0f,  // bottom-left
		-0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f, 0.0f, 1.0f,// top-left
		// Front face
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f, // bottom-left
		 0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 0.0f,  // bottom-right
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f,  // top-right
		 0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 1.0f, 1.0f, // top-right
		-0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 1.0f,  // top-left
		-0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f, 0.0f, 0.0f,  // bottom-left
		// Left face				 
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top-right
		-0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // top-left
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  // bottom-left
		-0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom-left
		-0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f,  0.0f,  // bottom-right
		-0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top-right
		// Right face				 	    	   		  
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f, // top-left
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  1.0f, // top-right         
		 0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,  // bottom-right
		 0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f,  0.0f,  // top-left
		 0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f,  0.0f, // bottom-left     
		// Bottom face		  			    	   		  
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // top-right
		 0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  1.0f, // top-left
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f,// bottom-left
		 0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f,  0.0f, // bottom-left
		-0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  0.0f, // bottom-right
		-0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f,  1.0f, // top-right
		// Top face			  			    	   		  
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,// top-left
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // bottom-right
		 0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  1.0f, // top-right     
		 0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f,  0.0f, // bottom-right
		-0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  1.0f,// top-left
		-0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f,  0.0f // bottom-left      
	};

	unsigned int ShadowMapVBO;
	glGenVertexArrays(1, &m_ShadowMapVAO);
	glGenBuffers(1, &ShadowMapVBO);
	glBindVertexArray(m_ShadowMapVAO);
	glBindBuffer(GL_ARRAY_BUFFER, ShadowMapVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(QuadVertices), &QuadVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int QuadVBO;
	glGenVertexArrays(1, &m_QuadVAO);
	glGenBuffers(1, &QuadVBO);
	glBindVertexArray(m_QuadVAO);
	glBindBuffer(GL_ARRAY_BUFFER, QuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PlaneVertices), &PlaneVertices, GL_STATIC_DRAW);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);

	unsigned int CubeVBO;
	glGenVertexArrays(1, &m_CubeVAO);
	glGenBuffers(1, &CubeVBO);
	glBindVertexArray(m_CubeVAO);
	glBindBuffer(GL_ARRAY_BUFFER, CubeVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(CubeVertices), CubeVertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initFBO()
{
	glGenFramebuffers(1, &m_DrawShadowMapFBO);

	glGenTextures(1, &m_ShadowMapTex);
	glBindTexture(GL_TEXTURE_2D, m_ShadowMapTex);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 1024, 1024, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
	glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
	glBindFramebuffer(GL_FRAMEBUFFER, m_DrawShadowMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_ShadowMapTex, 0);
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initCallback()
{
	glfwSetCursorPosCallback(m_pGLFWWindow, mouse_callback);
	glfwSetScrollCallback(m_pGLFWWindow, scroll_callback);
}

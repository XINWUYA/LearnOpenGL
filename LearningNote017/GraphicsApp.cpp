#include "GraphicsApp.h"
#include <functional>
#include <map>

std::map<GLFWwindow*, CGraphicsApp*> g_Screen;

CGraphicsApp::CGraphicsApp(int vWindowWidth, int vWindowHeight, std::string& vWindowName) : m_pGLFWWindow(nullptr), m_pShader(nullptr), m_pRockShader(nullptr), m_pTexture(nullptr), m_pTexture1(nullptr), m_pPlanetModel(nullptr), m_pRockModel(nullptr)
{
	__initGLFWWindow(vWindowWidth, vWindowHeight, vWindowName);
	_ASSERTE(glGetError() == GL_NO_ERROR);
	g_Screen[m_pGLFWWindow] = this;
	
	m_WindowWidth = vWindowWidth;
	m_WindowHeight = vWindowHeight;
	float m_LastX = vWindowWidth / 2.0f;
	float m_LastY = vWindowHeight / 2.0f;
}

CGraphicsApp::~CGraphicsApp()
{
	g_Screen.erase(m_pGLFWWindow);
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteVertexArrays(1, &m_LightVAO);
	glfwTerminate();
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::init()
{
	__initCallback();
	__initShader();
	//__initTexture();
	__importModel();
	__generateRockMatrices();
	__initVAO();
}

//**********************************************************************************
//FUNCTION:
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

//**********************************************************************************
//FUNCTION:
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
		float CurrentFrame = glfwGetTime();
		m_DeltaTime = CurrentFrame - m_LastFrame;
		m_LastFrame = CurrentFrame;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		__processInput(m_pGLFWWindow);
		if (m_Polygon) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_pShader->useShaderProgram();
		m_pShader->setVec3("uCameraPos", CCamera::get_mutable_instance().getCameraPosition());
		m_pShader->setFloat("Material.m_Shininess", 32.0f);
		m_pShader->setVec3("DirectLight.m_Direction", -0.2f, -1.0f, -0.3f);
		m_pShader->setVec3("DirectLight.m_Ambient", 0.05f, 0.05f, 0.05f);
		m_pShader->setVec3("DirectLight.m_Diffuse", 0.4f, 0.4f, 0.4f);
		m_pShader->setVec3("DirectLight.m_Specular", 0.5f, 0.5f, 0.5f);
		//glm::vec3 LightColor(sin(CurrentFrame * 2.0f), sin(CurrentFrame * 0.7f), sin(CurrentFrame * 1.3f));
		glm::vec3 LightColor(1.0f);
		glm::vec3 Diffuse = LightColor * glm::vec3(0.5f);
		glm::vec3 Ambient = Diffuse * glm::vec3(0.2f);
		m_pShader->setVec3 ("PointLights[0].m_Position", m_LightPosition[0]);
		m_pShader->setVec3 ("PointLights[0].m_Ambient", Ambient);
		m_pShader->setVec3 ("PointLights[0].m_Diffuse", Diffuse);
		m_pShader->setVec3 ("PointLights[0].m_Specular", 1.0f, 1.0f, 1.0f);
		m_pShader->setFloat("PointLights[0].m_Constant", 1.0f);
		m_pShader->setFloat("PointLights[0].m_Liner", 0.09f);
		m_pShader->setFloat("PointLights[0].m_Quadratic", 0.032f);

		m_pShader->setVec3 ("PointLights[1].m_Position", m_LightPosition[1]);
		m_pShader->setVec3 ("PointLights[1].m_Ambient", Ambient);
		m_pShader->setVec3 ("PointLights[1].m_Diffuse", Diffuse);
		m_pShader->setVec3 ("PointLights[1].m_Specular", 1.0f, 1.0f, 1.0f);
		m_pShader->setFloat("PointLights[1].m_Constant", 1.0f);
		m_pShader->setFloat("PointLights[1].m_Liner", 0.09f);
		m_pShader->setFloat("PointLights[1].m_Quadratic", 0.032f);

		m_pShader->setVec3 ("PointLights[2].m_Position", m_LightPosition[2]);
		m_pShader->setVec3 ("PointLights[2].m_Ambient", Ambient);
		m_pShader->setVec3 ("PointLights[2].m_Diffuse", Diffuse);
		m_pShader->setVec3 ("PointLights[2].m_Specular", 1.0f, 1.0f, 1.0f);
		m_pShader->setFloat("PointLights[2].m_Constant", 1.0f);
		m_pShader->setFloat("PointLights[2].m_Liner", 0.09f);
		m_pShader->setFloat("PointLights[2].m_Quadratic", 0.032f);

		m_pShader->setVec3 ("PointLights[3].m_Position", m_LightPosition[3]);
		m_pShader->setVec3 ("PointLights[3].m_Ambient", Ambient);
		m_pShader->setVec3 ("PointLights[3].m_Diffuse", Diffuse);
		m_pShader->setVec3 ("PointLights[3].m_Specular", 1.0f, 1.0f, 1.0f);
		m_pShader->setFloat("PointLights[3].m_Constant", 1.0f);
		m_pShader->setFloat("PointLights[3].m_Liner", 0.09f);
		m_pShader->setFloat("PointLights[3].m_Quadratic", 0.032f);

		m_pShader->setVec3 ("SpotLight.m_Position", CCamera::get_mutable_instance().getCameraPosition());
		m_pShader->setVec3 ("SpotLight.m_Direction", CCamera::get_mutable_instance().getCameraFront());
		m_pShader->setFloat("SpotLight.m_InnerCutOffAngle", glm::cos(glm::radians(12.5f)));
		m_pShader->setFloat("SpotLight.m_OuterCutOffAngle", glm::cos(glm::radians(20.0f)));
		m_pShader->setVec3 ("SpotLight.m_Ambient", Ambient);
		m_pShader->setVec3 ("SpotLight.m_Diffuse", Diffuse);
		m_pShader->setVec3 ("SpotLight.m_Specular", 1.0f, 1.0f, 1.0f);
		m_pShader->setFloat("SpotLight.m_Constant", 1.0f);
		m_pShader->setFloat("SpotLight.m_Liner", 0.09f);
		m_pShader->setFloat("SpotLight.m_Quadratic", 0.032f);
		glm::mat4 Projection = glm::perspective(glm::radians(CCamera::get_mutable_instance().getCameraZoom()), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 1000.0f);
		m_pShader->setMat4("uProjection", Projection);
		glm::mat4 View = CCamera::get_mutable_instance().getViewMatrix();
		m_pShader->setMat4("uView", View);
		glm::mat4 Model;
		Model = glm::translate(Model, glm::vec3(0.0f, -1.75f, 0.0f));
		Model = glm::scale(Model, glm::vec3(4.0f));
		m_pShader->setMat4("uModel", Model);
		m_pPlanetModel->drawModel(m_pShader, 5);

		m_pRockShader->useShaderProgram();
		m_pRockShader->setVec3("uCameraPos", CCamera::get_mutable_instance().getCameraPosition());
		m_pRockShader->setFloat("Material.m_Shininess", 32.0f);
		m_pRockShader->setVec3("DirectLight.m_Direction", -0.2f, -1.0f, -0.3f);
		m_pRockShader->setVec3("DirectLight.m_Ambient", 0.05f, 0.05f, 0.05f);
		m_pRockShader->setVec3("DirectLight.m_Diffuse", 0.4f, 0.4f, 0.4f);
		m_pRockShader->setVec3("DirectLight.m_Specular", 0.5f, 0.5f, 0.5f);
		m_pRockShader->setVec3("PointLights[0].m_Position", m_LightPosition[0]);
		m_pRockShader->setVec3("PointLights[0].m_Ambient", Ambient);
		m_pRockShader->setVec3("PointLights[0].m_Diffuse", Diffuse);
		m_pRockShader->setVec3("PointLights[0].m_Specular", 1.0f, 1.0f, 1.0f);
		m_pRockShader->setFloat("PointLights[0].m_Constant", 1.0f);
		m_pRockShader->setFloat("PointLights[0].m_Liner", 0.09f);
		m_pRockShader->setFloat("PointLights[0].m_Quadratic", 0.032f);

		m_pRockShader->setVec3("PointLights[1].m_Position", m_LightPosition[1]);
		m_pRockShader->setVec3("PointLights[1].m_Ambient", Ambient);
		m_pRockShader->setVec3("PointLights[1].m_Diffuse", Diffuse);
		m_pRockShader->setVec3("PointLights[1].m_Specular", 1.0f, 1.0f, 1.0f);
		m_pRockShader->setFloat("PointLights[1].m_Constant", 1.0f);
		m_pRockShader->setFloat("PointLights[1].m_Liner", 0.09f);
		m_pRockShader->setFloat("PointLights[1].m_Quadratic", 0.032f);

		m_pRockShader->setVec3("PointLights[2].m_Position", m_LightPosition[2]);
		m_pRockShader->setVec3("PointLights[2].m_Ambient", Ambient);
		m_pRockShader->setVec3("PointLights[2].m_Diffuse", Diffuse);
		m_pRockShader->setVec3("PointLights[2].m_Specular", 1.0f, 1.0f, 1.0f);
		m_pRockShader->setFloat("PointLights[2].m_Constant", 1.0f);
		m_pRockShader->setFloat("PointLights[2].m_Liner", 0.09f);
		m_pRockShader->setFloat("PointLights[2].m_Quadratic", 0.032f);

		m_pRockShader->setVec3("PointLights[3].m_Position", m_LightPosition[3]);
		m_pRockShader->setVec3("PointLights[3].m_Ambient", Ambient);
		m_pRockShader->setVec3("PointLights[3].m_Diffuse", Diffuse);
		m_pRockShader->setVec3("PointLights[3].m_Specular", 1.0f, 1.0f, 1.0f);
		m_pRockShader->setFloat("PointLights[3].m_Constant", 1.0f);
		m_pRockShader->setFloat("PointLights[3].m_Liner", 0.09f);
		m_pRockShader->setFloat("PointLights[3].m_Quadratic", 0.032f);

		m_pRockShader->setVec3("SpotLight.m_Position", CCamera::get_mutable_instance().getCameraPosition());
		m_pRockShader->setVec3("SpotLight.m_Direction", CCamera::get_mutable_instance().getCameraFront());
		m_pRockShader->setFloat("SpotLight.m_InnerCutOffAngle", glm::cos(glm::radians(12.5f)));
		m_pRockShader->setFloat("SpotLight.m_OuterCutOffAngle", glm::cos(glm::radians(20.0f)));
		m_pRockShader->setVec3("SpotLight.m_Ambient", Ambient);
		m_pRockShader->setVec3("SpotLight.m_Diffuse", Diffuse);
		m_pRockShader->setVec3("SpotLight.m_Specular", 1.0f, 1.0f, 1.0f);
		m_pRockShader->setFloat("SpotLight.m_Constant", 1.0f);
		m_pRockShader->setFloat("SpotLight.m_Liner", 0.09f);
		m_pRockShader->setFloat("SpotLight.m_Quadratic", 0.032f);
		m_pRockShader->setMat4("uProjection", Projection);
		m_pRockShader->setMat4("uView", View);
		m_pRockShader->setInt("Material.m_Diffuse0", 0);
		m_pRockShader->setInt("Material.m_Specular0", 1);
		m_pRockShader->setInt("Material.m_Ambient0", 2);
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_pRockModel->getTextureLoadedSet()[0].m_TextureID);
		for (unsigned int i = 0; i < m_pRockModel->getMeshSet().size(); i++)
		{
			glBindVertexArray(m_pRockModel->getMeshSet()[i].getVAO());
			glDrawElementsInstanced(GL_TRIANGLES, m_pRockModel->getMeshSet()[i].getIndicesSet().size(), GL_UNSIGNED_INT, 0, ROCK_AMOUNT);
			glBindVertexArray(0);
		}

		glfwPollEvents();
		glfwSwapBuffers(m_pGLFWWindow);
	}
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::mouse_callback(GLFWwindow* vWindow, double vXPos, double vYPos)
{
	if (m_FirstMouse)
	{
		m_LastX = vXPos;
		m_LastY = vYPos;
		m_FirstMouse = false;
	}

	float XOffset = vXPos - m_LastX;
	float YOffset = m_LastY - vYPos;

	m_LastX = vXPos;
	m_LastY = vYPos;
	CCamera::get_mutable_instance().processMouseMovement(XOffset, YOffset);
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::scroll_callback(GLFWwindow* vWindow, double vXOffset, double vYOffset)
{
	CCamera::get_mutable_instance().processMouseScroll(vYOffset);
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
	//glfwWindowHint(GLFW_DECORATED, GL_FALSE);//没有边框和标题栏
	m_pGLFWWindow = glfwCreateWindow(vWindowWidth, vWindowHeight, vWindowName.c_str(), nullptr, nullptr);
	if (m_pGLFWWindow == NULL)
	{
		std::cout << "Failed to Create GLFW Window." << std::endl;
		glfwTerminate();
		return false;
	}
	//glfwShowWindow(m_pGLFWWindow);//可有可无？
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
	m_pShader = new CShader();
	m_pShader->addShader("../ShaderSources/LN017/PlanetVertShader.glsl", VERTEX_SHADER);
	m_pShader->addShader("../ShaderSources/LN017/PlanetFragShader.glsl", FRAGMENT_SHADER);
	m_pShader->createShaderProgram();

	m_pRockShader = new CShader();
	m_pRockShader->addShader("../ShaderSources/LN017/RockVertShader.glsl", VERTEX_SHADER);
	m_pRockShader->addShader("../ShaderSources/LN017/RockFragShader.glsl", FRAGMENT_SHADER);
	m_pRockShader->createShaderProgram();
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initTexture()
{
	m_pTexture = new CTexture(GL_TEXTURE_2D, "../TextureSources/container2.png");
	m_pTexture1 = new CTexture(GL_TEXTURE_2D, "../TextureSources/container2_specular.png");
}


//***********************************************************
//FUNCTION:
void CGraphicsApp::__importModel()
{
	m_pPlanetModel = new CModel("../ModelSources/planet/planet.obj");
	m_pRockModel   = new CModel("../ModelSources/rock/rock.obj");
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initVAO()
{
	for (unsigned int i = 0; i < m_pRockModel->getMeshSet().size(); i++)
	{
		unsigned int VAO = m_pRockModel->getMeshSet()[i].getVAO();
		glBindVertexArray(VAO);

		glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));
		glEnableVertexAttribArray(6);

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);
	}

	glBindVertexArray(0);
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initCallback()
{
	glfwSetCursorPosCallback(m_pGLFWWindow,	
		[](GLFWwindow * vWindow, double vXPos, double vYPos) 
		{
			auto it = g_Screen.find(vWindow);
			if (it == g_Screen.end()) return;
			CGraphicsApp* g = it->second;
			g->mouse_callback(vWindow, vXPos, vYPos);
		}
	);
	glfwSetScrollCallback(m_pGLFWWindow, 
		[](GLFWwindow * vWindow, double vXOffset, double vYOffset) 
		{
			auto it = g_Screen.find(vWindow);
			if (it == g_Screen.end()) return;
			CGraphicsApp * g = it->second;
			g->scroll_callback(vWindow, vXOffset, vYOffset);
		}
	);
}

//***********************************************************
//FUNCTION:
void CGraphicsApp::__processInput(GLFWwindow* vWindow)
{
	if (glfwGetKey(vWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(vWindow, true);
	if (glfwGetKey(vWindow, GLFW_KEY_P) == GLFW_PRESS)
		m_Polygon = !m_Polygon;

	if (glfwGetKey(vWindow, GLFW_KEY_W) == GLFW_PRESS)
		CCamera::get_mutable_instance().processKeyBoard(FORWARD, m_DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_S) == GLFW_PRESS)
		CCamera::get_mutable_instance().processKeyBoard(BACKWARD, m_DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_A) == GLFW_PRESS)
		CCamera::get_mutable_instance().processKeyBoard(LEFT, m_DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_D) == GLFW_PRESS)
		CCamera::get_mutable_instance().processKeyBoard(RIGHT, m_DeltaTime);
}

//***********************************************************
//FUNCTION:
void CGraphicsApp::__generateRockMatrices()
{
	std::srand(glfwGetTime());
	float Radius = 150.0f;
	float Offset = 25.0f;
	float Displacement = 0;
	for (unsigned int i = 0; i < ROCK_AMOUNT; i++)
	{
		glm::mat4 Model;
		float Angle = (float)i / (float)ROCK_AMOUNT * 360.0f;

		Displacement = (std::rand() % (int)(2 * Offset * 100)) / 100.0f - Offset;
		float X = sin(Angle) * Radius + Displacement;
		Displacement = (std::rand() % (int)(2 * Offset * 100)) / 100.0f - Offset;
		float Y = Displacement * 0.4f;
		Displacement = (std::rand() % (int)(2 * Offset * 100)) / 100.0f - Offset;
		float Z = cos(Angle) * Radius + Displacement;
		Model = glm::translate(Model, glm::vec3(X, Y, Z));

		float Scale = (std::rand() % 20) / 100.0f + 0.05f;
		Model = glm::scale(Model, glm::vec3(Scale));

		float RotateAngle = (std::rand() % 360);
		//Model = glm::rotate(Model, RotateAngle, glm::vec3(0.4f, 0.6f, 0.8f));
		Model = glm::rotate(Model, RotateAngle, glm::normalize(glm::vec3(std::rand(), std::rand(), std::rand())));
		
		m_RockMatrices[i] = Model;
	}

	unsigned int VBO;
	//glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);
	//glBindVertexArray(m_VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, ROCK_AMOUNT * sizeof(glm::mat4), &m_RockMatrices[0], GL_STATIC_DRAW);
}

#include "GraphicsApp.h"

CGraphicsApp::CGraphicsApp(int vWindowWidth, int vWindowHeight, std::string& vWindowName) : m_pGLFWWindow(nullptr), m_pShader(nullptr), m_pShaderFur(nullptr), m_pLightShader(nullptr), m_pTexture(nullptr), m_pTexture1(nullptr), m_pModel(nullptr)
{
	__initGLFWWindow(vWindowWidth, vWindowHeight, vWindowName);
	_ASSERTE(glGetError() == GL_NO_ERROR);
	
	
	m_WindowWidth = vWindowWidth;
	m_WindowHeight = vWindowHeight;
}

CGraphicsApp::~CGraphicsApp()
{
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
	__initTexture();
	__initVAO();
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::importModel(const std::string & vFilePath)
{
	m_pModel = new CModel(vFilePath);
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
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(m_pGLFWWindow);
		if (Polygon) glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		else glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		m_pShader->useShaderProgram();
		m_pShader->setFloat("uTime", glfwGetTime());
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
		glm::mat4 Projection = glm::perspective(glm::radians(CCamera::get_mutable_instance().getCameraZoom()), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
		m_pShader->setMat4("uProjection", Projection);
		glm::mat4 View = CCamera::get_mutable_instance().getViewMatrix();
		m_pShader->setMat4("uView", View);
		glm::mat4 NanosuitModel;
		NanosuitModel = glm::translate(NanosuitModel, glm::vec3(0.0f, -1.75f, 0.0f));
		NanosuitModel = glm::scale(NanosuitModel, glm::vec3(0.2f));
		m_pShader->setMat4("uModel", NanosuitModel);
		m_pModel->drawModel(m_pShader, 5);

		m_pShaderFur->useShaderProgram();
		m_pShaderFur->setFloat("uTime", glfwGetTime());
		m_pShaderFur->setMat4("uProjection", Projection);
		m_pShaderFur->setMat4("uView", View);
		m_pShaderFur->setMat4("uModel", NanosuitModel);
		m_pModel->drawModel(m_pShader, 5);

		/*m_pLightShader->useShaderProgram();
		m_pLightShader->setVec3("uObjectColor", LightColor);
		m_pLightShader->setMat4("uProjection", Projection);
		m_pLightShader->setMat4("uView", View);
		glBindVertexArray(m_LightVAO);
		for (auto i = 0; i < 4; i++)
		{
			glm::mat4 LightModel;
			LightModel = glm::translate(LightModel, m_LightPosition[i]);
			LightModel = glm::scale(LightModel, glm::vec3(0.2f));
			m_pLightShader->setMat4("uModel", LightModel);
			glDrawArrays(GL_TRIANGLES, 0, 36);
		}*/
		
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
	m_pShader = new CShader();
	m_pShader->addShader("../ShaderSources/LN016/NanosuitVertShader.glsl", VERTEX_SHADER);
	m_pShader->addShader("../ShaderSources/LN016/NanosuitGeomShader.glsl", GEOMETRY_SHADER);
	m_pShader->addShader("../ShaderSources/LN016/NanosuitFragShader.glsl", FRAGMENT_SHADER);
	m_pShader->createShaderProgram();

	m_pShaderFur = new CShader();
	m_pShaderFur->addShader("../ShaderSources/LN016/NanosuitVertShader.glsl", VERTEX_SHADER);
	m_pShaderFur->addShader("../ShaderSources/LN016/NanosuitGeomShaderFur.glsl", GEOMETRY_SHADER);
	m_pShaderFur->addShader("../ShaderSources/LN016/NanosuitFragShaderFur.glsl", FRAGMENT_SHADER);
	m_pShaderFur->createShaderProgram();

	m_pLightShader = new CShader();
	m_pLightShader->addShader("../ShaderSources/LN016/LightVertShader.glsl", VERTEX_SHADER);
	m_pLightShader->addShader("../ShaderSources/LN016/LightFragShader.glsl", FRAGMENT_SHADER);
	m_pLightShader->createShaderProgram();
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initTexture()
{
	m_pTexture = new CTexture(GL_TEXTURE_2D, "../TextureSources/container2.png");
	m_pTexture1 = new CTexture(GL_TEXTURE_2D, "../TextureSources/container2_specular.png");
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initVAO()
{
	float Vertices[] = {
		//Position            //Texture    //Normal
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  0.0f, -1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,  0.0f,  0.0f, -1.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,  0.0f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  0.0f, 1.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f, -1.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f, -1.0f,  0.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f, -1.0f,  0.0f,  0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  1.0f,  0.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  1.0f,  0.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  1.0f,  0.0f,  0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,  0.0f, -1.0f,  0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,  0.0f, -1.0f,  0.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,  0.0f,  1.0f,  0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,  0.0f,  1.0f,  0.0f
	};
	
	unsigned int VBO;
	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(5 * sizeof(float)));
	glEnableVertexAttribArray(2);

	glGenVertexArrays(1, &m_LightVAO);
	glBindVertexArray(m_LightVAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initCallback()
{
	glfwSetCursorPosCallback(m_pGLFWWindow, mouse_callback);
	glfwSetScrollCallback(m_pGLFWWindow, scroll_callback);
}

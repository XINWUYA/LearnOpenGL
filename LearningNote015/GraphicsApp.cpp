#include "GraphicsApp.h"

CGraphicsApp::CGraphicsApp(int vWindowWidth, int vWindowHeight, std::string& vWindowName) : m_pGLFWWindow(nullptr), m_pShaderR(nullptr), m_pShaderG(nullptr), m_pShaderB(nullptr), m_pShaderY(nullptr), m_pTexture(nullptr)
{
	__initGLFWWindow(vWindowWidth, vWindowHeight, vWindowName);
	_ASSERTE(glGetError() == GL_NO_ERROR);
	
	
	m_WindowWidth = vWindowWidth;
	m_WindowHeight = vWindowHeight;
}

CGraphicsApp::~CGraphicsApp()
{
	glDeleteVertexArrays(1, &m_VAO);
	glDeleteBuffers(1, &m_VBO);
	glDeleteBuffers(1, &m_UBO);
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
	unsigned int UniformBlockIndexR = glGetUniformBlockIndex(m_pShaderR->getShaderProgram(), "Matrices");
	unsigned int UniformBlockIndexG = glGetUniformBlockIndex(m_pShaderG->getShaderProgram(), "Matrices");
	unsigned int UniformBlockIndexB = glGetUniformBlockIndex(m_pShaderB->getShaderProgram(), "Matrices");
	unsigned int UniformBlockIndexY = glGetUniformBlockIndex(m_pShaderY->getShaderProgram(), "Matrices");
	glUniformBlockBinding(m_pShaderR->getShaderProgram(), UniformBlockIndexR, 0);
	glUniformBlockBinding(m_pShaderG->getShaderProgram(), UniformBlockIndexG, 0);
	glUniformBlockBinding(m_pShaderB->getShaderProgram(), UniformBlockIndexB, 0);
	glUniformBlockBinding(m_pShaderY->getShaderProgram(), UniformBlockIndexY, 0);
	
	glm::mat4 Projection = glm::perspective(glm::radians(CCamera::get_mutable_instance().getCameraZoom()), (float)m_WindowWidth / (float)m_WindowHeight, 0.1f, 100.0f);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), glm::value_ptr(Projection));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	while (!glfwWindowShouldClose(m_pGLFWWindow) && !glfwGetKey(m_pGLFWWindow, GLFW_KEY_ESCAPE))
	{
		float CurrentFrame = glfwGetTime();
		DeltaTime = CurrentFrame - LastFrame;
		LastFrame = CurrentFrame;

		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		processInput(m_pGLFWWindow);

		m_pTexture->bindImageTexture(GL_TEXTURE0);

		glm::mat4 View = CCamera::get_mutable_instance().getViewMatrix();
		glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
		glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), glm::value_ptr(View));

		m_pShaderR->useShaderProgram();
		m_pShaderR->setInt("uTexture", 0);
		glBindVertexArray(m_VAO);
		glm::mat4 Model;
		Model = glm::translate(Model, m_CubePosition[0]);
		float Angle = 0.0;
		Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_pShaderR->setMat4("uModel", Model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		m_pShaderG->useShaderProgram();
		m_pShaderG->setInt("uTexture", 0);
		glBindVertexArray(m_VAO);
		Model = glm::mat4();
		Model = glm::translate(Model, m_CubePosition[1]);
		Angle = 20.0;
		Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_pShaderG->setMat4("uModel", Model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		m_pShaderB->useShaderProgram();
		m_pShaderB->setInt("uTexture", 0);
		glBindVertexArray(m_VAO);
		Model = glm::mat4();
		Model = glm::translate(Model, m_CubePosition[2]);
		Angle = 20.0;
		Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_pShaderB->setMat4("uModel", Model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

		m_pShaderY->useShaderProgram();
		m_pShaderY->setInt("uTexture", 0);
		glBindVertexArray(m_VAO);
		Model = glm::mat4();
		Model = glm::translate(Model, m_CubePosition[3]);
		Angle = 20.0;
		Model = glm::rotate(Model, glm::radians(Angle), glm::vec3(1.0f, 0.3f, 0.5f));
		m_pShaderY->setMat4("uModel", Model);
		glDrawArrays(GL_TRIANGLES, 0, 36);

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
	m_pShaderR = new CShader();
	m_pShaderR->addShader("../ShaderSources/LN015/vertShader.glsl", VERTEX_SHADER);
	m_pShaderR->addShader("../ShaderSources/LN015/fragShaderR.glsl", FRAGMENT_SHADER);
	m_pShaderR->createShaderProgram();

	m_pShaderG = new CShader();
	m_pShaderG->addShader("../ShaderSources/LN015/vertShader.glsl", VERTEX_SHADER);
	m_pShaderG->addShader("../ShaderSources/LN015/fragShaderG.glsl", FRAGMENT_SHADER);
	m_pShaderG->createShaderProgram();

	m_pShaderB = new CShader();
	m_pShaderB->addShader("../ShaderSources/LN015/vertShader.glsl", VERTEX_SHADER);
	m_pShaderB->addShader("../ShaderSources/LN015/fragShaderB.glsl", FRAGMENT_SHADER);
	m_pShaderB->createShaderProgram();

	m_pShaderY = new CShader();
	m_pShaderY->addShader("../ShaderSources/LN015/vertShader.glsl", VERTEX_SHADER);
	m_pShaderY->addShader("../ShaderSources/LN015/fragShaderY.glsl", FRAGMENT_SHADER);
	m_pShaderY->createShaderProgram();
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initTexture()
{
	m_pTexture = new CTexture(GL_TEXTURE_2D, "../TextureSources/container.jpg");
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initVAO()
{
	float Vertices[] = {
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
		 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
		-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
		-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};

	glGenVertexArrays(1, &m_VAO);
	glGenBuffers(1, &m_VBO);

	glBindVertexArray(m_VAO);

	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glGenBuffers(1, &m_UBO);
	glBindBuffer(GL_UNIFORM_BUFFER, m_UBO);
	glBufferData(GL_UNIFORM_BUFFER, 2 * sizeof(glm::mat4), NULL, GL_STATIC_DRAW);
	glBindBufferRange(GL_UNIFORM_BUFFER, 0, m_UBO, 0, 2 * sizeof(glm::mat4));
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	glBindVertexArray(0);
}

//**********************************************************************************
//FUNCTION:
void CGraphicsApp::__initCallback()
{
	glfwSetCursorPosCallback(m_pGLFWWindow, mouse_callback);
	glfwSetScrollCallback(m_pGLFWWindow, scroll_callback);
}

void CGraphicsApp::__processInput(GLFWwindow * vWindow)
{
}

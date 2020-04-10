#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "CShader.h"

int main()
{
	glfwInit();

	GLFWwindow* pWindow = glfwCreateWindow(800, 600, "Hello, Triangle", NULL, NULL);
	if (pWindow == NULL)
	{
		std::cout << "Failed to Create GLFW Window." << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(pWindow);
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD." << std::endl;
		return -1;
	}

	CShader Shader;
	Shader.addShader("../ShaderSources/LN002/vertShader.glsl", VertexShader);
	Shader.addShader("../ShaderSources/LN002/fragShader.glsl", FragmentShader);
	Shader.createShaderProgram();

	GLfloat Vertices[] = {
		-0.5f, -0.5f, 0.0f,
		 0.5f, -0.5f, 0.0f,
		 0.0f,  0.5f, 0.0f
	};

	GLuint VAO, VBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertices), Vertices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);
	glEnableVertexAttribArray(0);

	glBindVertexArray(0);

	//glEnable(GL_PROGRAM_POINT_SIZE);
	while (!glfwWindowShouldClose(pWindow) && !glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Shader.useShaderProgram();
		glBindVertexArray(VAO);
		glDrawArrays(GL_TRIANGLES, 0, 3);
		//glDrawArrays(GL_POINTS, 0, 3);

		glfwPollEvents();
		glfwSwapBuffers(pWindow);
	}

	glfwTerminate();
	return 0;
}
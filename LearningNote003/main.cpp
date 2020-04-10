#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include "Shader.h"
#include "Texture.h"

#include "stb_image.h"

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

	glewInit();

	CShader Shader;
	Shader.addShader("../ShaderSources/LN003/vertShader.glsl", VERTEX_SHADER);
	Shader.addShader("../ShaderSources/LN003/fragShader.glsl", FRAGMENT_SHADER);
	Shader.createShaderProgram();

	float vertices[] = {
		 1.0f,  1.0f, 0.0f,  1.0f, 1.0f,
		 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
		-1.0f,  1.0f, 0.0f,  0.0f, 1.0f
	};
	unsigned int indices[] = {
		0, 1, 3,
		1, 2, 3
	};
	unsigned int VBO, VAO, EBO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0);

	CTexture Texture1(GL_TEXTURE_2D, "../TextureSources/wall.jpg");
	CTexture Texture2(GL_TEXTURE_2D, "../TextureSources/container.jpg");

	Shader.useShaderProgram();
	glUniform1i(glGetUniformLocation(Shader.getShaderProgram(), "sTexture1"), 0);
	glUniform1i(glGetUniformLocation(Shader.getShaderProgram(), "sTexture2"), 1);

	while (!glfwWindowShouldClose(pWindow) && !glfwGetKey(pWindow, GLFW_KEY_ESCAPE))
	{
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		Texture1.bindImageTexture(GL_TEXTURE0);
		Texture2.bindImageTexture(GL_TEXTURE1);

		Shader.useShaderProgram();
		glBindVertexArray(VAO);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

		glfwPollEvents();
		glfwSwapBuffers(pWindow);
	}

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);

	glfwTerminate();
	return 0;
}
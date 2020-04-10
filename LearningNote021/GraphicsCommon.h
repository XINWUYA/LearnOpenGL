#pragma once
#include <GLFW\glfw3.h>

extern float DeltaTime;
extern float LastTime;

void mouse_callback(GLFWwindow* vWindow, double vXPos, double vYPos);
void scroll_callback(GLFWwindow* vWindow, double vXOffset, double vYOffset);
void processInput(GLFWwindow *vWindow);

enum class ECursorMode
{
	NORMAL,
	HIDDEN,
	DISABLE
};

template<class T>
void SAFE_DELETE(T*& p)
{
	if (p != nullptr) delete p;
	p = nullptr;
	*(&p) = nullptr;
}
#pragma once
#include <GLFW\glfw3.h>

extern float DeltaTime;
extern float LastFrame;
extern bool Polygon;

void mouse_callback(GLFWwindow* vWindow, double vXPos, double vYPos);
void scroll_callback(GLFWwindow* vWindow, double vXOffset, double vYOffset);
void key_callback(GLFWwindow* vWindow, int vKey, int vScanCode, int vAction, int vMode);
void processInput(GLFWwindow *vWindow);

enum ECursorMode
{
	NORMAL,
	HIDDEN,
	DISABLE
};
#include "GraphicsCommon.h"
#include "Camera.h"

static float LastX = 800.0f / 2.0f;
static float LastY = 600.0f / 2.0f;
static bool FirstMouse = true;
float DeltaTime = 0.0f;
float LastFrame = 0.0f;

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
	CCamera::get_mutable_instance().processMouseMovement(XOffset, YOffset);
}

//**********************************************************************************
//FUNCTION:
void scroll_callback(GLFWwindow* vWindow, double vXOffset, double vYOffset)
{
	CCamera::get_mutable_instance().processMouseScroll(vYOffset);
}

//**********************************************************************************
//FUNCTION:
void processInput(GLFWwindow *vWindow)
{
	if (glfwGetKey(vWindow, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(vWindow, true);

	if (glfwGetKey(vWindow, GLFW_KEY_W) == GLFW_PRESS)
		CCamera::get_mutable_instance().processKeyBoard(FORWARD, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_S) == GLFW_PRESS)
		CCamera::get_mutable_instance().processKeyBoard(BACKWARD, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_A) == GLFW_PRESS)
		CCamera::get_mutable_instance().processKeyBoard(LEFT, DeltaTime);
	if (glfwGetKey(vWindow, GLFW_KEY_D) == GLFW_PRESS)
		CCamera::get_mutable_instance().processKeyBoard(RIGHT, DeltaTime);
}
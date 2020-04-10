#include "Camera.h"

const float YAW = -90.0f;
const float PITCH = 0.0f;
const float MOVEMENT_SPEED = 2.5f;
const float MOUSE_SENSITIVITY = 0.1f;
const float ZOOM = 45.0f;

CCamera::CCamera() : m_CameraPosition(glm::vec3(0.0f, -1.5f, 200.0f)), m_CameraFront(glm::vec3(0.0f, 0.0f, -1.0f)), m_CameraUp(glm::vec3(0.0f, 1.0f, 0.0f)), m_IsOpenYawCamera(true)
{

	m_Yaw = YAW;
	m_Pitch = PITCH;
	m_MovementSpeed = MOVEMENT_SPEED;
	m_MouseSensitivity = MOUSE_SENSITIVITY;
	m_Zoom = ZOOM;
	m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

	__updateCamera();
}

CCamera::~CCamera(void)
{

}

//**********************************************************************************
//FUNCTION:
glm::mat4 CCamera::getViewMatrix() const
{
	return glm::lookAt(m_CameraPosition, m_CameraPosition + m_CameraFront, m_CameraUp);
}

//**********************************************************************************
//FUNCTION:
float CCamera::getCameraZoom() const
{
	return m_Zoom;
}

//**********************************************************************************
//FUNCTION:
glm::vec3 CCamera::getCameraPosition() const
{
	return m_CameraPosition;
}

//**********************************************************************************
//FUNCTION:
glm::vec3 CCamera::getCameraFront() const
{
	return m_CameraFront;
}

//**********************************************************************************
//FUNCTION:
void CCamera::processKeyBoard(ECameraMovement vMovementDirection, GLfloat vDeltaTime)
{
	if (0.0f == vDeltaTime) vDeltaTime = 0.05f;
	GLfloat Velocity = m_MovementSpeed * vDeltaTime;

	if (vMovementDirection == FORWARD)  m_CameraPosition += m_CameraFront * Velocity;
	if (vMovementDirection == BACKWARD) m_CameraPosition -= m_CameraFront * Velocity;
	if (vMovementDirection == LEFT)		m_CameraPosition -= m_CameraRight * Velocity;
	if (vMovementDirection == RIGHT)	m_CameraPosition += m_CameraRight * Velocity;
}

//**********************************************************************************
//FUNCTION:
void CCamera::processMouseMovement(GLfloat vXOffset, GLfloat vYOffset, GLboolean vConstrainPicth /*= true*/)
{
	vXOffset *= m_MouseSensitivity;
	vYOffset *= m_MouseSensitivity;
	m_Yaw += vXOffset;
	m_Pitch += vYOffset;

	if (vConstrainPicth)
	{
		if (m_Pitch >  89.0f) m_Pitch =  89.0f;
		if (m_Pitch < -89.0f) m_Pitch = -89.0f;
	}

	__updateCamera();
}

//**********************************************************************************
//FUNCTION:
void CCamera::processMouseScroll(GLfloat vYOffset)
{
	if (m_Zoom >= 1.0f && m_Zoom <= 45.0f)
		m_Zoom -= vYOffset;

	if (m_Zoom <=  1.0f) m_Zoom =  1.0f;
	if (m_Zoom >= 45.0f) m_Zoom = 45.0f;
}

//**********************************************************************************
//FUNCTION:
void CCamera::__updateCamera()
{
	glm::vec3 TempFront;

	if (m_IsOpenYawCamera)
	{
		TempFront.x = cos(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));
		TempFront.y = sin(glm::radians(m_Pitch));
		TempFront.z = sin(glm::radians(m_Yaw)) * cos(glm::radians(m_Pitch));

		m_CameraFront = glm::normalize(TempFront);
		m_CameraRight = glm::normalize(glm::cross(m_CameraFront, m_WorldUp));
		m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
	}
	else
	{
		if (m_Yaw >=  60.0f) m_Yaw =  60.0f;
		if (m_Yaw <= -60.0f) m_Yaw = -60.0f;

		TempFront.x = cos(glm::radians(m_Pitch));
		TempFront.y = sin(glm::radians(m_Pitch));
		TempFront.z = cos(glm::radians(m_Pitch));

		m_CameraFront = glm::normalize(TempFront);
		m_CameraRight = glm::normalize(glm::cross(m_CameraFront, (m_WorldUp + glm::vec3(tan(glm::radians(m_Yaw)), 0, 0))));
		m_CameraUp = glm::normalize(glm::cross(m_CameraRight, m_CameraFront));
	}
}


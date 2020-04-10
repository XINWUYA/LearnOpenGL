#pragma once
#include <GLM/glm.hpp>
#include <boost/serialization/singleton.hpp>

enum ECameraMovement
{
	FORWARD = 0,
	BACKWARD,
	LEFT,
	RIGHT
};
	
class CCamera : public boost::serialization::singleton<CCamera>
{
public:
	CCamera();
	~CCamera(void);

	glm::mat4 getViewMatrix() const;
	float	  getCameraZoom() const;
	glm::vec3 getCameraPosition() const;

	void processKeyBoard(ECameraMovement vMovementDirection, GLfloat vDeltaTime);
	void processMouseMovement(GLfloat vXOffset, GLfloat vYOffset, GLboolean vConstrainPicth = true);
	void processMouseScroll(GLfloat vYOffset);

private:
	void __updateCamera();

	glm::vec3 m_CameraPosition;
	glm::vec3 m_CameraFront;
	glm::vec3 m_CameraUp;
	glm::vec3 m_CameraRight;
	glm::vec3 m_WorldUp;
	
	float m_Yaw, m_Pitch, m_Roll;
	float m_MovementSpeed;
	float m_MouseSensitivity;
	float m_Zoom;

	bool m_IsOpenYawCamera;
};
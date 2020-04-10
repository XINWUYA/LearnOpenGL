#pragma once
#include <GL/glew.h>
#include <GLM/glm.hpp>
#include "Singleton.h"

namespace Xinwuya
{
	enum class ECameraMovement
	{
		FORWARD = 0,
		BACKWARD,
		LEFT,
		RIGHT
	};

	class CCamera : public CSingleton<CCamera>
	{
		friend class CSingleton<CCamera>;
	public:
		CCamera();
		~CCamera(void) = default;

		glm::mat4 getViewMatrix() const;
		float	  getCameraZoom() const;
		glm::vec3 getCameraPosition() const;

		void processKeyBoard(ECameraMovement vMovementDirection, GLfloat vDeltaTime);
		void processMouseMovement(GLfloat vXOffset, GLfloat vYOffset, GLboolean vConstrainPicth = true);
		void processMouseScroll(GLfloat vYOffset);

	private:
		void __updateCamera();

		glm::vec3 m_CameraPosition = glm::vec3(0.0f, 0.0f, 3.0f);
		glm::vec3 m_CameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
		glm::vec3 m_CameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
		glm::vec3 m_CameraRight = glm::vec3(0.0f);
		glm::vec3 m_WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);

		float m_Yaw = -90.0f;
		float m_Pitch = 0.0f;
		float m_Roll = 0.0f;
		float m_MovementSpeed = 2.5f;
		float m_MouseSensitivity = 0.1f;
		float m_Zoom = 45.0f;

		bool m_IsOpenYawCamera = true;
	};
}
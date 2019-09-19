#pragma once
#ifndef SJMCAMERA
#define SJMCAMERA

#include <glm/glm.hpp>

namespace SJM {

	enum Camera_Movement {
		FORWARD,
		BACKWARD,
		LEFT,
		RIGHT
	};


	// Default camera values
	const float YAW = -90.0f;
	const float PITCH = 0.0f;
	const float SPEED = 2.5f;
	const float SENSITIVITY = 0.1f;
	const float ZOOM = 45.0f;
	const float NEARDISTANCE = 0.1f;
	const float FARDISTANCE = 50;

	class Camera {
	public:
		// 摄像机位置
		glm::vec3 pos;
		// 摄像机正面方向
		glm::vec3 front;
		// 摄像机指向上的位置
		glm::vec3 up;
		// 摄像机指向右的方向
		glm::vec3 right;
		// 在世界坐标系上摄像机指向上的方向(为模糊方向,用于后续求right和up)
		glm::vec3 worldUp;

		float fovAngle;
		float aspect;
		float nearDistance, farDistance;

		// 俯仰角和偏航角
		float Yaw, Pitch;

		// 摄像机移动速度以及鼠标转向时的敏感度
		float MovementSpeed;
		float MouseSensitivity;

		Camera(
			glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f),
			glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f),
			float yaw = YAW,
			float pitch = PITCH
		);

		glm::mat4x4 getViewMatrix();
		glm::mat4x4 getProjectionMatrix();

		// 处理摄像机视角/移动相关

		/*
			direction: 摄像机移动的方向枚举,有forward/back/right/left四种
			deltaTime: 两次更新之间间隔时间
		*/
		void ProcessKeyboard(Camera_Movement direction,float deltaTime);

		/*
			xoffset/yoffset: 鼠标距离上次移动的距离
		*/
		void ProcessMouseMovement(float xoffset,float yoffset,bool constrainPitch=true);

		// 处理鼠标滚轮
		void ProcessMouseScroll(float yoffset);

	private :
		// 更新摄像机的front向量(根据当前的俯仰角和偏航角)
		void updateCameraVectors();
	};
}


#endif // !SJMCAMERA

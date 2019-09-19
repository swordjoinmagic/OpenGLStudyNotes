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
		// �����λ��
		glm::vec3 pos;
		// ��������淽��
		glm::vec3 front;
		// �����ָ���ϵ�λ��
		glm::vec3 up;
		// �����ָ���ҵķ���
		glm::vec3 right;
		// ����������ϵ�������ָ���ϵķ���(Ϊģ������,���ں�����right��up)
		glm::vec3 worldUp;

		float fovAngle;
		float aspect;
		float nearDistance, farDistance;

		// �����Ǻ�ƫ����
		float Yaw, Pitch;

		// ������ƶ��ٶ��Լ����ת��ʱ�����ж�
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

		// ����������ӽ�/�ƶ����

		/*
			direction: ������ƶ��ķ���ö��,��forward/back/right/left����
			deltaTime: ���θ���֮����ʱ��
		*/
		void ProcessKeyboard(Camera_Movement direction,float deltaTime);

		/*
			xoffset/yoffset: �������ϴ��ƶ��ľ���
		*/
		void ProcessMouseMovement(float xoffset,float yoffset,bool constrainPitch=true);

		// ����������
		void ProcessMouseScroll(float yoffset);

	private :
		// �����������front����(���ݵ�ǰ�ĸ����Ǻ�ƫ����)
		void updateCameraVectors();
	};
}


#endif // !SJMCAMERA

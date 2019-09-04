#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "Camera.h"

using namespace SJM;

Camera::Camera(
	glm::vec3 position,
	glm::vec3 up,
	float yaw,
	float pitch) : 
	front(glm::vec3(0.0f, 0.0f, -1.0f)), 
	MovementSpeed(SPEED),
	MouseSensitivity(SENSITIVITY), 
	fovAngle(ZOOM),
	nearDistance(NEARDISTANCE),
	farDistance(FARDISTANCE),
	aspect(1.0f){

	this->pos = position;
	this->worldUp = up;
	this->Yaw = yaw;
	this->Pitch = pitch;

	updateCameraVectors();
}

glm::mat4x4 Camera::getViewMatrix() {
	return glm::lookAt(
		pos,pos+ front,up
	);
}

glm::mat4x4 Camera::getProjectionMatrix() {
	return glm::perspective(
		glm::radians(fovAngle),aspect,
		nearDistance,farDistance);
}

// ����front/up/right����
// (�������ģ������ϵ������������������ϵ�µı�ʾ)
void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(front,worldUp));
	this->up = glm::normalize(glm::cross(right,front));
}

// ������̲���,���ڱ任������Ŀռ�λ��
void Camera::ProcessKeyboard(Camera_Movement direction, float deltaTime) {
	float velocity = MovementSpeed * deltaTime;
	if (direction == FORWARD)
		pos += front * velocity;
	if (direction == BACKWARD)
		pos -= front * velocity;
	if (direction == LEFT)
		pos -= right * velocity;
	if (direction == RIGHT)
		pos += right * velocity;
}

// ����������,���ڱ任�������fov�Ƕ�
void Camera::ProcessMouseScroll(float yoffset) {
	if (fovAngle >= 1.0f && fovAngle <=45.0f)
		fovAngle -= yoffset;
	if (fovAngle <= 1.0f)
		fovAngle = 1.0f;
	if (fovAngle >= 45.0f)
		fovAngle = 45.0f;
}

// ��������ƶ�,�����ڱ任��������ӽǵĲ���
void Camera::ProcessMouseMovement(float xoffset, float yoffset, bool constrainPitch) {	
	xoffset *= MouseSensitivity;
	yoffset *= MouseSensitivity;

	Yaw += xoffset;
	Pitch += yoffset;

	if (constrainPitch) {
		if (Pitch > 89.0f) Pitch = 89.0f;
		if (Pitch < -89.0f) Pitch = -89.0f;
	}

	updateCameraVectors();
}
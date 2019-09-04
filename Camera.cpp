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

// 更新front/up/right向量
// (即摄像机模型坐标系的三个轴在世界坐标系下的表示)
void Camera::updateCameraVectors() {
	glm::vec3 front;
	front.x = cos(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	front.y = sin(glm::radians(Pitch));
	front.z = sin(glm::radians(Yaw)) * cos(glm::radians(Pitch));
	this->front = glm::normalize(front);
	this->right = glm::normalize(glm::cross(front,worldUp));
	this->up = glm::normalize(glm::cross(right,front));
}

// 处理键盘操作,用于变换摄像机的空间位置
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

// 处理鼠标滚轮,用于变换摄像机的fov角度
void Camera::ProcessMouseScroll(float yoffset) {
	if (fovAngle >= 1.0f && fovAngle <=45.0f)
		fovAngle -= yoffset;
	if (fovAngle <= 1.0f)
		fovAngle = 1.0f;
	if (fovAngle >= 45.0f)
		fovAngle = 45.0f;
}

// 处理鼠标移动,即用于变换摄像机的视角的操作
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
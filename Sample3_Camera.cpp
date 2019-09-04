#include "main.h"
#include "Sample3_Camera.h"
#include <glm/gtc/type_ptr.hpp>

void Sample3::initVertex() {
	// ����VAO���󲢰�
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	// ������������,���������뵽����(VBO)��
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
	};
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);
	
	// pos
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);
	// ���ò����ö�������
	glEnableVertexAttribArray(0);
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Sample3::init() {
	initVertex();
	// ��ʼ��shader
	shader = std::make_shared<Shader>("Shader/Sample3/vertex.glsl", "Shader/Sample3/fragment.glsl");
	// ��ʼ������
	image = std::make_shared<SJM::Image>("Image/container.jpg");

	shader->setTexture2D("MainTex",*image,0);

	// ���������
	camera = std::make_shared<SJM::Camera>(
		glm::vec3(0, 0, 3),
		glm::vec3(0, 0, -1.0),
		glm::vec3(0, 1.0f, 0),
		45,1,0.1f,100.0f
		);
}

void Sample3::render() {

	glUseProgram(shader->ID);
	// ��VAO
	glBindVertexArray(VAO);

	// ģ�;���
	glm::mat4x4 modelMatrix = glm::mat4x4(1.0f);
	glm::vec3 yAxis(0,1,0);
	modelMatrix = glm::rotate(modelMatrix,(float)glfwGetTime(),yAxis);
	modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime(), glm::vec3(1,0,0));
	// ��ù۲����
	glm::mat4x4 viewMatrix = camera->getViewMatrix();
	// ���ͶӰ����
	glm::mat4x4 projMatrix = camera->getProjectionMatrix();

	// ����������������
	shader->setTexture2D("MainTex", *image, 0);
	// ����mvp����
	shader->setMatrix4x4("m",glm::value_ptr(modelMatrix));
	shader->setMatrix4x4("v",glm::value_ptr(viewMatrix));
	shader->setMatrix4x4("p",glm::value_ptr(projMatrix));

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// ����ͼ��
	glDrawArrays(GL_TRIANGLES,0,36);
}

void Sample3::onDestory() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1,&VBO);
}

void Sample3::processInput(GLFWwindow *window,float deltaTime) {
	float speed = 5;
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
		camera->pos -= glm::normalize(glm::cross(camera->front, camera->up)) * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
		camera->pos += glm::normalize(glm::cross(camera->front, camera->up)) * speed * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
		camera->pos += speed * camera->front * deltaTime;
	if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
		camera->pos -= speed * camera->front * deltaTime;
}

void Sample3::mouseCallBack(GLFWwindow* window, double xpos, double ypos) {

	if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) != GLFW_PRESS) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = true;
		return;
	}

	if (firstMouse) {
		lastX = xpos;
		lastY = ypos;
		firstMouse = false;
	}


	// ���㵱ǰ֡����һ֡���λ�õ�ƫ����
	float xOffset = xpos - lastX;
	float yOffset = lastY - ypos;

	lastX = xpos;
	lastY = ypos;

	float sensitivity = 0.05f;
	xOffset *= sensitivity;
	yOffset *= sensitivity;

	yaw += xOffset;
	pitch += yOffset;

	if (pitch > 89.0) pitch = 89.0f;
	if (pitch < -89.0) pitch = -89.0f;

	// ͨ�������Ǻ�ƫ���Ǽ��㷽������
	glm::vec3 front;
	front.x = cos(glm::radians(pitch))*cos(glm::radians(yaw));
	front.y = sin(glm::radians(pitch));
	front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

	camera->front = glm::normalize(front);
}
void Sample3::scrollCallBack(GLFWwindow* window, double xOffset, double yOffset) {
	if (camera->fovAngle >= 1.0f && camera->fovAngle <= 45.0f)
		camera->fovAngle -= yOffset;
	if (camera->fovAngle <= 1.0f) camera->fovAngle = 1.0f;
	if (camera->fovAngle >= 45.0f) camera->fovAngle = 45.0f;
}

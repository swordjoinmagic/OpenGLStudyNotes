#pragma once
#ifndef SAMPLE3
#define SAMPLE3

#include <memory>
#include "Shader.h"
#include "Image.h"
#include "Camera.h"

class Sample3 {
private:
	std::shared_ptr<Shader> shader;
	unsigned int VAO;
	unsigned int VBO;
	std::shared_ptr<SJM::Image> image;

	int angle = 0;
	float lastX = 400, lastY = 300;
	// 偏航角和俯仰角
	float yaw = 0,pitch = 0;
	bool firstMouse = true;

	// 初始化顶点数据
	void initVertex();
public:
	std::shared_ptr<SJM::Camera> camera;
	void init();
	void render();
	void onDestory();
	//void processInput(GLFWwindow *window,float deltaTime);
	//void mouseCallBack(GLFWwindow* window, double xpos, double ypos);
	//void scrollCallBack(GLFWwindow* window,double xOffset,double yOffset);
};

#endif // !SAMPLER#

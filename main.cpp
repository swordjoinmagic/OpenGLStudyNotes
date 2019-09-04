#include "iostream"
#include "main.h"
#include "Sample1_DrawTriangle.h"
#include"Sample2_Texture.h"
#include "Sample3_Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void processMouseInput(GLFWwindow* window, double xpos, double ypos);
void processMouseScroll(GLFWwindow* window, double xOffset, double yOffset);

// 上次更新时间
float lastUpdateTime;

Sample3 sample;

float deltaTime = 0;

int main()
{
	// 初始化glfw库
	glfwInit();
	// 设置OpenGL版本
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// 设置OpenGL渲染模式为核心渲染模式
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// 创建窗口对象
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// 通知GLFW将我们窗口的上下文设置为当前线程的主上下文
	glfwMakeContextCurrent(window);

	// 初始化GLAD(一个用于管理OpenGL函数指针的库)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// 设置视口,需要注意的是,当窗口大小发生变化时,视口要同时变化
	glViewport(0, 0, 800, 600);

	// 设置回调函数,当窗体Size变化时调用的函数
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	sample.init();

	// 隐藏鼠标光标
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// 设置鼠标的回调函数
	glfwSetCursorPosCallback(window,processMouseInput);
	// 设置鼠标滚轮的回调函数
	glfwSetScrollCallback(window,processMouseScroll);

	lastUpdateTime = glfwGetTime();

	// 渲染循环
	while (!glfwWindowShouldClose(window))
	{
		// 处理输入
		processInput(window);

		float nowTime = glfwGetTime();
		deltaTime = nowTime - lastUpdateTime;
		lastUpdateTime = nowTime;		

		// 清理颜色缓冲
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#pragma region 渲染指令
		sample.render();
		#pragma endregion

		// 交换双缓冲
		glfwSwapBuffers(window);
		// 检查是否有触发某些事件
		glfwPollEvents();
	}

	sample.onDestory();

	// 释放所有资源
	glfwTerminate();

	return 0;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
	if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
		glfwSetWindowShouldClose(window, true);

	sample.processInput(window,deltaTime);
}

void processMouseInput(GLFWwindow* window, double xpos, double ypos) {
	sample.mouseCallBack(window,xpos,ypos);
}

void processMouseScroll(GLFWwindow* window, double xOffset, double yOffset) {
	sample.scrollCallBack(window,xOffset,yOffset);
}

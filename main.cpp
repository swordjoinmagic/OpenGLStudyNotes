#include "iostream"
#include "main.h"
#include "Sample1_DrawTriangle.h"
#include"Sample2_Texture.h"
#include "Sample3_Camera.h"

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void processMouseInput(GLFWwindow* window, double xpos, double ypos);
void processMouseScroll(GLFWwindow* window, double xOffset, double yOffset);

// �ϴθ���ʱ��
float lastUpdateTime;

Sample3 sample;

float deltaTime = 0;

int main()
{
	// ��ʼ��glfw��
	glfwInit();
	// ����OpenGL�汾
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// ����OpenGL��ȾģʽΪ������Ⱦģʽ
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	//glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

	// �������ڶ���
	GLFWwindow* window = glfwCreateWindow(800, 600, "LearnOpenGL", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	// ֪ͨGLFW�����Ǵ��ڵ�����������Ϊ��ǰ�̵߳���������
	glfwMakeContextCurrent(window);

	// ��ʼ��GLAD(һ�����ڹ���OpenGL����ָ��Ŀ�)
	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
	{
		std::cout << "Failed to initialize GLAD" << std::endl;
		return -1;
	}

	// �����ӿ�,��Ҫע�����,�����ڴ�С�����仯ʱ,�ӿ�Ҫͬʱ�仯
	glViewport(0, 0, 800, 600);

	// ���ûص�����,������Size�仯ʱ���õĺ���
	glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
	
	sample.init();

	// ���������
	//glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	// �������Ļص�����
	glfwSetCursorPosCallback(window,processMouseInput);
	// ���������ֵĻص�����
	glfwSetScrollCallback(window,processMouseScroll);

	lastUpdateTime = glfwGetTime();

	// ��Ⱦѭ��
	while (!glfwWindowShouldClose(window))
	{
		// ��������
		processInput(window);

		float nowTime = glfwGetTime();
		deltaTime = nowTime - lastUpdateTime;
		lastUpdateTime = nowTime;		

		// ������ɫ����
		glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		#pragma region ��Ⱦָ��
		sample.render();
		#pragma endregion

		// ����˫����
		glfwSwapBuffers(window);
		// ����Ƿ��д���ĳЩ�¼�
		glfwPollEvents();
	}

	sample.onDestory();

	// �ͷ�������Դ
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

#include "main.h"
#include "Sample3_Camera.h"
#include <glm/gtc/type_ptr.hpp>

void Sample3::initVertex() {
	// 生成VAO对象并绑定
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	// 创建顶点数据,并将它输入到缓冲(VBO)中
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
	// 设置并启用顶点数据
	glEnableVertexAttribArray(0);
	// uv
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
}

void Sample3::init() {
	initVertex();
	// 初始化shader
	shader = std::make_shared<Shader>("Shader/Sample3/vertex.glsl", "Shader/Sample3/fragment.glsl");
	// 初始化纹理
	image = std::make_shared<SJM::Image>("Image/container.jpg");

	shader->setTexture2D("MainTex",*image,0);

}

void Sample3::render() {

	glUseProgram(shader->ID);
	// 绑定VAO
	glBindVertexArray(VAO);

	// 模型矩阵
	glm::mat4x4 modelMatrix = glm::mat4x4(1.0f);
	glm::vec3 yAxis(0,1,0);
	modelMatrix = glm::rotate(modelMatrix,(float)glfwGetTime(),yAxis);
	modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime(), glm::vec3(1,0,0));
	// 获得观察矩阵
	glm::mat4x4 viewMatrix = camera->getViewMatrix();
	// 获得投影矩阵
	glm::mat4x4 projMatrix = camera->getProjectionMatrix();

	// 设置纹理并激活纹理
	shader->setTexture2D("MainTex", *image, 0);
	// 设置mvp矩阵
	shader->setMatrix4x4("m",glm::value_ptr(modelMatrix));
	shader->setMatrix4x4("v",glm::value_ptr(viewMatrix));
	shader->setMatrix4x4("p",glm::value_ptr(projMatrix));

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// 绘制图形
	glDrawArrays(GL_TRIANGLES,0,36);
}

void Sample3::onDestory() {
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1,&VBO);
}

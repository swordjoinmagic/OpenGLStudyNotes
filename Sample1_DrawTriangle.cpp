#include "main.h"
#include "Sample1_DrawTriangle.h"
#include <iostream>

void Sample1::init() {
	
	shader = std::make_shared<Shader>("Shader/Sample1/vertexShader.glsl", "Shader/Sample1/fragmentShader.glsl");

	// 初始化顶点缓冲
	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f,   // 右上角
	//	0.5f, -0.5f, 0.0f,  // 右下角
	//	-0.5f, -0.5f, 0.0f, // 左下角
	//	-0.5f, 0.5f, 0.0f   // 左上角
	//};
	float vertices[] = {
		-0.5f,  0.5f,  0.0,
		 0.5f,  0.5f,  0.0,
		 0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f
	};

	// 向OpenGL申请缓冲
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 顶点索引数组
	//unsigned int indices[] = { // 注意索引从0开始! 
	//	0, 1, 3, // 第一个三角形
	//	1, 2, 3  // 第二个三角形
	//};
	unsigned int indices[] = {
		0,1,2,
		2,3,4
	};

	// 根据顶点索引数组生成顶点缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// 绑定顶点索引缓冲,并把数据输进去
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置顶点属性指针
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// 启用顶点数据
	glEnableVertexAttribArray(0);

	// 重置当前绑定的缓冲和VAO对象
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sample1::render() {
	// 绘制物体
	glUseProgram(shader->ID);	

	// 绑定顶点数组对象(用于保存顶点属性的对象)
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
	
}

void Sample1::destory() {

}
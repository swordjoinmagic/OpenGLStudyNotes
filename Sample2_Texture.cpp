#include "main.h"
#include "Sample2_Texture.h"
#include "stb_image.h"
#include "Image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace SJM;

void Sample2::initVertex() {
	// 顶点数据(包括位置/颜色/uv属性)
	float vertices[] = {
		//     ---- 位置 ----       ---- 颜色 ----     - 纹理坐标 -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // 右上
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // 右下
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // 左下
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // 左上
	};
	unsigned int VBO;

	// 创建顶点数组对象,并绑定
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//创建顶点缓冲对象
	glGenBuffers(1, &VBO);
	// 绑定顶点缓冲对象,并把顶点数据输入进缓冲
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// 生成顶点索引缓冲
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	// 根据顶点索引数组生成顶点缓冲对象
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// 绑定顶点索引缓冲,并把数据输进去
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// 设置各顶点属性指针,并启用顶点属性

	// 设置顶点位置
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// 设置顶点颜色
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// 设置顶点uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Sample2::createTexture() {
	texture1 = std::make_shared<Image>("Image/container.jpg");
	texture2 = std::make_shared<Image>("Image/29126173.bmp");
}

void Sample2::init() {
	// 初始化着色器
	shader = std::make_shared<Shader>("Shader/Sample2/vertex.glsl", "Shader/Sample2/fragment.glsl");

	initVertex();
	createTexture();

	glUseProgram(shader->ID);
	shader->setTexture2D("texture1", *texture1, 0);
	shader->setTexture2D("texture2", *texture2, 1);
}

void Sample2::render() {

	glUseProgram(shader->ID);

	// 设置MVP矩阵

	// 世界坐标/旋转/缩放
	glm::vec3 position(0.5f,0,0);
	glm::vec3 scale(1,1,1);
	scale *= 0.5f;
	glm::mat4x4 mMatrix = glm::mat4(1.0f);
	// 顺序: 缩放->旋转->平移
	mMatrix = glm::scale(mMatrix,scale);
	mMatrix = glm::translate(mMatrix,position);

	shader->setMatrix4x4("mvp",glm::value_ptr(mMatrix));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texture1->textureID);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texture2->textureID);

	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
}

void Sample2::onDestory() {
	glDeleteVertexArrays(1,&VAO);
}
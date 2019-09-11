#include "Sample12_Instances.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <strstream>
#include <string>


const float quadVertices[] = {
	// 位置          // 颜色
	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	-0.05f, -0.05f,  0.0f, 0.0f, 1.0f,

	-0.05f,  0.05f,  1.0f, 0.0f, 0.0f,
	 0.05f, -0.05f,  0.0f, 1.0f, 0.0f,
	 0.05f,  0.05f,  0.0f, 1.0f, 1.0f
};

glm::vec2 translations[100];

void Sample12::init() {
	// 初始化顶点数组对象
	glGenVertexArrays(1, &quadVAO);
	glBindVertexArray(quadVAO);
	unsigned int VBO;
	glGenBuffers(1, &VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), quadVertices, GL_STATIC_DRAW);

	// 启用并设置顶点属性

	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);

	// color
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(2 * sizeof(float)));

	// 初始化着色器
	objShader = std::make_shared<Shader>("Shader/Sample12 Instances/vWithArray.glsl", "Shader/Sample12 Instances/f.glsl");

	// 初始化偏移数组
	int index = 0;
	float offset = 0.1f;
	for (int y = -10; y < 10; y += 2) {
		for (int x = -10; x < 10; x += 2) {
			glm::vec2 translation;
			translation.x = (float)x / 10.0f + offset;
			translation.y = (float)y / 10.0f + offset;
			translations[index++] = translation;
		}
	}

	// 初始化实例数组
	unsigned int instanceVBO;
	glGenBuffers(1,&instanceVBO);
	glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(glm::vec2)*100,&translations[0],GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER,instanceVBO);
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,2*sizeof(float),(void*)0);
	glBindBuffer(GL_ARRAY_BUFFER,0);
	glVertexAttribDivisor(2,1);

	// 恢复
	glBindVertexArray(0);
}

void Sample12::render() {
	objShader->use();

	glBindVertexArray(quadVAO);
	glDrawArraysInstanced(GL_TRIANGLES,0,6,100);
}

void Sample12::onDestory() {}


#include "main.h"
#include "Sample1_DrawTriangle.h"
#include <iostream>

void Sample1::init() {
	
	shader = std::make_shared<Shader>("Shader/Sample1/vertexShader.glsl", "Shader/Sample1/fragmentShader.glsl");

	// ��ʼ�����㻺��
	//float vertices[] = {
	//	0.5f, 0.5f, 0.0f,   // ���Ͻ�
	//	0.5f, -0.5f, 0.0f,  // ���½�
	//	-0.5f, -0.5f, 0.0f, // ���½�
	//	-0.5f, 0.5f, 0.0f   // ���Ͻ�
	//};
	float vertices[] = {
		-0.5f,  0.5f,  0.0,
		 0.5f,  0.5f,  0.0,
		 0.0f,  0.0f,  0.0f,
		-0.5f, -0.5f,  0.0f,
		 0.5f, -0.5f,  0.0f
	};

	// ��OpenGL���뻺��
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ������������
	//unsigned int indices[] = { // ע��������0��ʼ! 
	//	0, 1, 3, // ��һ��������
	//	1, 2, 3  // �ڶ���������
	//};
	unsigned int indices[] = {
		0,1,2,
		2,3,4
	};

	// ���ݶ��������������ɶ��㻺�����
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// �󶨶�����������,�����������ȥ
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ���ö�������ָ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	// ���ö�������
	glEnableVertexAttribArray(0);

	// ���õ�ǰ�󶨵Ļ����VAO����
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Sample1::render() {
	// ��������
	glUseProgram(shader->ID);	

	// �󶨶����������(���ڱ��涥�����ԵĶ���)
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES,6, GL_UNSIGNED_INT, 0);
	
}

void Sample1::destory() {

}
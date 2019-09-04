#include "main.h"
#include "Sample2_Texture.h"
#include "stb_image.h"
#include "Image.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

using namespace SJM;

void Sample2::initVertex() {
	// ��������(����λ��/��ɫ/uv����)
	float vertices[] = {
		//     ---- λ�� ----       ---- ��ɫ ----     - �������� -
			 0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f,   // ����
			 0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f,   // ����
			-0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f,   // ����
			-0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f    // ����
	};
	unsigned int VBO;

	// ���������������,����
	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	//�������㻺�����
	glGenBuffers(1, &VBO);
	// �󶨶��㻺�����,���Ѷ����������������
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// ���ɶ�����������
	unsigned int indices[] = {
		0,1,3,
		1,2,3
	};
	// ���ݶ��������������ɶ��㻺�����
	unsigned int EBO;
	glGenBuffers(1, &EBO);
	// �󶨶�����������,�����������ȥ
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	// ���ø���������ָ��,�����ö�������

	// ���ö���λ��
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	// ���ö�����ɫ
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);
	// ���ö���uv
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Sample2::createTexture() {
	texture1 = std::make_shared<Image>("Image/container.jpg");
	texture2 = std::make_shared<Image>("Image/29126173.bmp");
}

void Sample2::init() {
	// ��ʼ����ɫ��
	shader = std::make_shared<Shader>("Shader/Sample2/vertex.glsl", "Shader/Sample2/fragment.glsl");

	initVertex();
	createTexture();

	glUseProgram(shader->ID);
	shader->setTexture2D("texture1", *texture1, 0);
	shader->setTexture2D("texture2", *texture2, 1);
}

void Sample2::render() {

	glUseProgram(shader->ID);

	// ����MVP����

	// ��������/��ת/����
	glm::vec3 position(0.5f,0,0);
	glm::vec3 scale(1,1,1);
	scale *= 0.5f;
	glm::mat4x4 mMatrix = glm::mat4(1.0f);
	// ˳��: ����->��ת->ƽ��
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
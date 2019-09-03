#include "main.h"
#include "Sample2_Texture.h"
#include "stb_image.h"

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
	// �����������
	glGenTextures(1, &texture);
	// ������,������������ָ��������õ�ǰ�󶨵�����
	glBindTexture(GL_TEXTURE_2D, texture);

	// ���õ�ǰ���������Ĳ���/����ģʽ

	// ���ò���ģʽΪrepeat
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	// �����������ģʽΪ���Թ���ģʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ��ȡ����
	int width, height, nrChannels;
	// ���ظ�ͼ��Ŀ��/�߶�/��ɫͨ������
	unsigned char *data = stbi_load("Image/container.jpg", &width, &height, &nrChannels, 0);

	if (data) {
		// ʹ��ǰ�������ͼƬ��������һ������
		glTexImage2D(
			GL_TEXTURE_2D,	// Ŀ������������
			0,				// �༶��Ա����ļ���
			GL_RGB,			// ����OpenGL������洢Ϊ���ָ�ʽ
			width, height,	// ����Ŀ�Ⱥ͸߶�
			0,				// Ӧ�����Ǳ���Ϊ0
			GL_RGB, GL_UNSIGNED_BYTE, // ������ԭͼ�ĸ�ʽ����������
			data			// ͼ������
		);
		// ���ɶ༶��Զ����
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "�޷���ȡ����" << std::endl;
	}
	// ������������,����ȡ������ͼ���ڴ��ͷ�
	stbi_image_free(data);
}

void Sample2::init() {
	// ��ʼ����ɫ��
	shader = std::make_shared<Shader>("Shader/Sample2/vertex.glsl", "Shader/Sample2/fragment.glsl");

	initVertex();
	createTexture();
}

void Sample2::render() {

	glUseProgram(shader->ID);

	glBindTexture(GL_TEXTURE_2D,texture);
	glBindVertexArray(VAO);

	glDrawElements(GL_TRIANGLES,6,GL_UNSIGNED_INT,0);
}

void Sample2::onDestory() {
	glDeleteVertexArrays(1,&VAO);
}
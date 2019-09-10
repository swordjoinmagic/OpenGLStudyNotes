#include "main.h"
#include "Image.h"
#include "stb_image.h"
#include <iostream>

SJM::Image::Image(const char* path):samplingMode(GL_REPEAT),filterMode(GL_LINEAR) {
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	// �����������
	glGenTextures(1, &textureID);
	// ������,������������ָ��������õ�ǰ�󶨵�����
	glBindTexture(GL_TEXTURE_2D, textureID);

	// ���õ�ǰ���������Ĳ���/����ģʽ

	// ���ò���ģʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, samplingMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, samplingMode);

	// �����������ģʽ
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);


	if (data) {
		// ʹ��ǰ�������ͼƬ��������һ������
		glTexImage2D(
			GL_TEXTURE_2D,	// Ŀ������������
			0,				// �༶��Զ����ļ���
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
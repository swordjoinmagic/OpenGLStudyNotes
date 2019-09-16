#include "CubeMap.h"
#include "stb_image.h"
#include <iostream>

using namespace SJM;

CubeMap::CubeMap(std::vector<std::string> path) {

	// ��������cubeMap�������
	glGenTextures(1,&textureID);
	glBindTexture(GL_TEXTURE_CUBE_MAP,textureID);

	int width, height, nrChannels;
	unsigned char* data;
	for (unsigned int i = 0; i < path.size();i++) {
		data = stbi_load(path[i].c_str(),&width,&height,&nrChannels,0);
		if (data) {
			glTexImage2D(
				GL_TEXTURE_CUBE_MAP_POSITIVE_X + i,
				0,
				GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data
			);
			stbi_image_free(data);
		}else{
			std::cout << "��������ʧ��,������·��Ϊ:"<<path[i] << std::endl;
		}
	}

	// �趨������������Ĺ��˺Ͳ���ģʽ
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
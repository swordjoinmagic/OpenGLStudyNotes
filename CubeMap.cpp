#include "CubeMap.h"
#include "stb_image.h"
#include <iostream>

using namespace SJM;

CubeMap::CubeMap(std::vector<std::string> path) {

	// 创建并绑定cubeMap纹理对象
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
			std::cout << "加载纹理失败,该纹理路径为:"<<path[i] << std::endl;
		}
	}

	// 设定该立方体纹理的过滤和采样模式
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
}
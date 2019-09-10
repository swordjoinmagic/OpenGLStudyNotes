#include "main.h"
#include "Image.h"
#include "stb_image.h"
#include <iostream>

SJM::Image::Image(const char* path):samplingMode(GL_REPEAT),filterMode(GL_LINEAR) {
	unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);

	// 生成纹理对象
	glGenTextures(1, &textureID);
	// 绑定纹理,后续所有纹理指令都可以配置当前绑定的纹理
	glBindTexture(GL_TEXTURE_2D, textureID);

	// 设置当前绑定纹理对象的采样/过滤模式

	// 设置采样模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, samplingMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, samplingMode);

	// 设置纹理过滤模式
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMode);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMode);


	if (data) {
		// 使用前面载入的图片数据生成一个纹理
		glTexImage2D(
			GL_TEXTURE_2D,	// 目标纹理缓冲类型
			0,				// 多级渐远纹理的级别
			GL_RGB,			// 告诉OpenGL把纹理存储为何种格式
			width, height,	// 纹理的宽度和高度
			0,				// 应该总是被设为0
			GL_RGB, GL_UNSIGNED_BYTE, // 定义了原图的格式和数据类型
			data			// 图像数据
		);
		// 生成多级渐远纹理
		glGenerateMipmap(GL_TEXTURE_2D);
	}
	else {
		std::cout << "无法读取纹理" << std::endl;
	}
	// 生成纹理对象后,将读取进来的图像内存释放
	stbi_image_free(data);
}
#pragma once
#ifndef SJMIMAGE
#define SJMIMAGE

#include <string>

namespace SJM {
	class Image {
	public:
		// 该纹理对象的ID
		unsigned int textureID;

		int width, height,nrChannels;
		// 过滤模式与采样模式
		int filterMode, samplingMode;
		Image(const char* path);
	};
}

#endif // !SJMIMAGE

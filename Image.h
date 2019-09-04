#pragma once
#ifndef SJMIMAGE
#define SJMIMAGE

#include <string>

namespace SJM {
	class Image {
	public:
		// ����������ID
		unsigned int textureID;

		int width, height,nrChannels;
		// ����ģʽ�����ģʽ
		int filterMode, samplingMode;
		Image(const char* path);
	};
}

#endif // !SJMIMAGE

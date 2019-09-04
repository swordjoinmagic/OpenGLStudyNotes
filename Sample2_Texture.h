#pragma once
#ifndef SAMPLE2
#define SAMPLE2

#include "main.h"
#include <memory>
#include "Shader.h"
#include "Image.h"
using namespace SJM;

class Sample2 {
private:
	std::shared_ptr<Shader> shader;
	void createTexture();
	void initVertex();
public:
	void init();
	void render();
	void onDestory();
private:
	unsigned int VAO;
	std::shared_ptr<Image> texture1;
	std::shared_ptr<Image> texture2;
};

#endif // !SAMPLE2

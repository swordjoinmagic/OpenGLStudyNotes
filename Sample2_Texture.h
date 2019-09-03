#pragma once
#ifndef SAMPLE2
#define SAMPLE2

#include "main.h"
#include <memory>
#include "Shader.h"

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
	unsigned int texture;
};

#endif // !SAMPLE2

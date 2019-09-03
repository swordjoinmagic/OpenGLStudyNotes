#pragma once
#ifndef SAMPLE1
#define SAMPLE1

#include "Shader.h"

class Sample1 {	
private:
	std::shared_ptr<Shader> shader;
	unsigned int VBO, VAO;
public:
	Sample1() {}
	void init();
	void render();
	void destory();
};
#endif 
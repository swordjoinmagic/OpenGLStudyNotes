#pragma once
#ifndef SAMPLE12
#define SAMPLE12

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>

using namespace SJM;

class Sample12 {
private:
	std::shared_ptr<Shader> objShader;
	std::shared_ptr<Image> objTexture;

	unsigned int quadVAO;
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE12

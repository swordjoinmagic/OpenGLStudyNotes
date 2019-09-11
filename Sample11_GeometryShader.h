#pragma once
#ifndef SAMPLE11
#define SAMPLE11

#include "Camera.h"
#include "Shader.h"
#include "Image.h"
#include <memory>
#include "Model.h"

using namespace SJM;

class Sample11 {
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Shader> explodeShader;
	std::shared_ptr<Model> explodeModel;
	std::shared_ptr<Image> explodeTexture;
	unsigned int VAO;
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE11

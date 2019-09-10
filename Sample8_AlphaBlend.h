#pragma once
#ifndef SAMPLE8
#define SAMPLE8

#include <memory>
#include "Shader.h"
#include "Image.h"
#include "Camera.h"
#include "Model.h"

using namespace SJM;

class Sample8 {
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Image> alphaTextTexture;
	std::shared_ptr<Image> alphaBlendTexture;
	std::shared_ptr<Model> quad;
public:
	void init();
	void render();
	void onDestory();
	std::shared_ptr<Camera> camera;
};


#endif // !SAMPLE8

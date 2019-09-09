#pragma once
#ifndef SAMPLE5
#define SAMPLE5

#include <memory>
#include "Shader.h"
#include "Image.h"
#include "Model.h"
#include "Camera.h"

using namespace SJM;

class Sample5 {
private:
	std::shared_ptr<Shader> shader;
	std::shared_ptr<Image> image;
	std::shared_ptr<Model> model;
public:
	void init();
	void render();
	void onDestory();

	std::shared_ptr<Camera> camera;
};

#endif // !SAMPLE5

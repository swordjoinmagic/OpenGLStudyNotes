#pragma once
#ifndef SAMPLE6
#define SAMPLE6

#include <memory>
#include "Shader.h"
#include "Camera.h"
#include "Model.h"

using namespace SJM;

class Sample6 {
private:
	std::shared_ptr<Model> cube;
	std::shared_ptr<Shader> shader;
public:
	void init();
	void render();
	void onDestory();
	std::shared_ptr<Camera> camera;
};

#endif // !SAMPLE6

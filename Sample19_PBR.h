#pragma once
#ifndef SAMPLE19
#define SAMPLE19

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>
#include "Cube.h"
#include <glm/glm.hpp>

class Sample19 {
private:
	std::shared_ptr<Shader> pbrShader;
	std::shared_ptr<Model> sphereModel;
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE19

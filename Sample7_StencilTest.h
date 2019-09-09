#pragma once
#ifndef SAMPLE7
#define SAMPLE7

#include <memory>
#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"

using namespace SJM;

class Sample7 {
private:
	std::shared_ptr<Model> model;
	// 用来正常绘制箱子的Shader
	std::shared_ptr<Shader> cubeShader;
	// 用来绘制边框的Shader
	std::shared_ptr<Shader> outLineShader;
	std::shared_ptr<Image> texture;
public:
	void init();
	void render();
	void onDestory();

	std::shared_ptr<Camera> camera;
};

#endif // !SAMPLE7

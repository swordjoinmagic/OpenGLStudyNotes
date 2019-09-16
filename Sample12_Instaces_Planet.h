#pragma once
#ifndef SAMPLE12PLANET
#define SAMPLE12PLANET

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>

class Sample12Planet {
private:
	std::shared_ptr<Model> planet;
	std::shared_ptr<Model> rock;
	std::shared_ptr<Shader> rockShader;
	std::shared_ptr<Shader> planetShader;
	std::shared_ptr<Image> rockTexture;
	std::shared_ptr<Image> planetTexture;


public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();	
};

#endif // !SAMPLE12PLANET

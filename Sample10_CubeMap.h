#pragma once
#ifndef SAMPLE10
#define SAMPLE10

#include "Shader.h"
#include "Camera.h"
#include "CubeMap.h"
#include "Model.h"
#include <memory>
#include "Cube.h"

using namespace SJM;

class Sample10 {
private:
	std::shared_ptr<Shader> skyBoxShader;
	std::shared_ptr<CubeMap> cubemap;
	std::shared_ptr<Cube> objModel;

	std::shared_ptr<Shader> objShader;
	std::shared_ptr<Image> objTexture;
	
	std::shared_ptr<Shader> reflectShader;
	std::shared_ptr<Shader> refractShader;

	unsigned int VAO;
public:
	void init();
	void render();
	void onDestory();
	std::shared_ptr<Camera> camera;
};

#endif // !SAMPLE10

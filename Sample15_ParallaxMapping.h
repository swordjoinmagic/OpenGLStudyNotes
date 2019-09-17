#pragma once
#ifndef SAMPLE15
#define SAMPLE15

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>
#include "Cube.h"
#include <glm/glm.hpp>

class Sample15 {
private:
	std::shared_ptr<Model> objModel;
	std::shared_ptr<Shader> objShader;
	std::shared_ptr<Image> diffuseMap;
	std::shared_ptr<Image> normalMap;
	std::shared_ptr<Image> heightMap;

	std::shared_ptr<glm::vec3> lightPos;
	unsigned int quadVAO;
	void RenderQuad();
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE15

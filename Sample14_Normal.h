#pragma once
#ifndef SAMPLE14
#define SAMPLE14

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>
#include "Cube.h"
#include <glm/glm.hpp>

class Sample14 {
private:
	unsigned int quadVAO;
	std::shared_ptr<Shader> objShader;	
	std::shared_ptr<Image> objTexture;
	std::shared_ptr<Image> normalTexture;
	std::shared_ptr<glm::vec3> lightDir;
	std::shared_ptr<Model> objModel;
	void initQuad();
	
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE14

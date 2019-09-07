#pragma once
#ifndef SAMPLE4
#define SAMPLE4

#include "main.h"
#include <memory>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"
#include "Image.h"

using namespace SJM;

class Sample4 {
private:
	// 此Shader用于渲染光源
	std::shared_ptr<Shader> whiteShader;
	// 此Shader用于渲染被光照射的物体
	std::shared_ptr<Shader> lightObjectShader;
	// 平行光光照Shader
	std::shared_ptr<Shader> directionalShader;
	// 点光源光照的Shader
	std::shared_ptr<Shader> pointShader;
	// 聚光灯光源的Shader
	std::shared_ptr<Shader> spotShader;
	// 被光照物体的主要纹理
	std::shared_ptr<Image> image;
	
	unsigned int VAO;
	unsigned int VBO;

public:
	std::shared_ptr<SJM::Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE4

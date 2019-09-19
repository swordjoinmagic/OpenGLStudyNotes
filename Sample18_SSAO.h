#pragma once
#ifndef SAMPLE18
#define SAMPLE18

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>
#include "Cube.h"
#include <glm/glm.hpp>

using namespace SJM;

class Sample18 {
private:
	// SSAO贴图
	unsigned int ssaoColorBuffer;
	// 用于存储SSAO贴图的帧缓冲
	unsigned int ssaoFBO;
	// 存储gBuffer的帧缓冲
	unsigned int gBuffer;
	// 各类gBuffer贴图对象(pos/normal/diffuseColor/depth等)
	unsigned int gPosDepth,gNormal,gAldebo;
	// 铺屏四边形顶点数组对象
	unsigned int screenQuadVAO;
	// 噪声贴图
	unsigned int noiseTexture;
	// 渲染gBuffer的Shader
	std::shared_ptr<Shader> gShader;
	// 渲染ssao的Shader
	std::shared_ptr<Shader> ssaoShader;
	// 渲染场景的Shader
	std::shared_ptr<Shader> postEffectShader;
	// 场景模型
	std::shared_ptr<Model> objModel;
	std::shared_ptr<Cube> cubeModel;
	std::vector<glm::vec3> ssaoKernel;

public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE18

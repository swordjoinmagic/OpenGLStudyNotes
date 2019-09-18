#pragma once
#ifndef SAMPLE17
#define SAMPLE17

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>
#include "Cube.h"
#include <glm/glm.hpp>

using namespace SJM;

class Sample17 {
private:
	// 铺屏四边形
	unsigned int screenQuadVAO;
	// 用于保存GBuffer的帧缓冲
	unsigned int gBuffer;
	// 用于保存物体坐标/法线/漫反射颜色(世界坐标)
	unsigned int gPosition, gNormal, gAlbedo;
	// 渲染gBuffer的shader
	std::shared_ptr<Shader> gShader;
	// 渲染场景的Shader(即被推迟的Piexel Shader)
	std::shared_ptr<Shader> deferredShader;
	// 测试用Shader
	std::shared_ptr<Shader> testShader;
	// 用于渲染光源的Shader
	std::shared_ptr<Shader> lightBoxShader;
	// 物体的光照纹理
	std::shared_ptr<Image> objTexture;
	// 物体模型
	std::shared_ptr<Cube> objModel;
	// 点光源位置与颜色
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	// 各物体的坐标
	std::vector<glm::vec3> objectPositions;
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE17

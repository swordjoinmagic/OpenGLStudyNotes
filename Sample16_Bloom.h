#pragma once
#ifndef SAMPLE16
#define SAMPLE16

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>
#include "Cube.h"
#include <glm/glm.hpp>

class Sample16 {
private:
	// 后处理帧缓冲
	unsigned int postEffectFBO;
	// 后处理帧缓冲中的颜色缓冲
	unsigned int postEffectColorBuffer[2];
	// 屏幕四边形的顶点数组对象
	unsigned int screenQuadVAO;
	// 用于高斯模糊的两个帧缓冲(两步高斯模糊,先横向再竖向)
	unsigned int blurFBO[2];
	unsigned int blurColorBuffer[2];
	// 用于Bloom(提取亮色的)Shader
	std::shared_ptr<Shader> bloomShader;
	// 高斯模糊Shader
	std::shared_ptr<Shader> blurShader;
	// 用于简单测试的图像
	std::shared_ptr<Image> texture;
	// 后处理Shader
	std::shared_ptr<Shader> postEffectShader;
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE16

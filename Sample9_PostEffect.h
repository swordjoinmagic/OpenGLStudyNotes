#pragma once
#ifndef SAMPLE9
#define SAMPLE9

#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include <memory>
#include "Camera.h"

using namespace SJM;

class Sample9 {
private:
	std::shared_ptr<Model> obj;
	std::shared_ptr<Shader> objShader;
	std::shared_ptr<Image> objTexture;
	// 用于渲染后处理画面的shader
	std::shared_ptr<Shader> quadShader;
	// 帧缓冲对象(包括各个附件(attachment),即深度缓冲/模板缓冲/颜色缓冲等)
	unsigned int framebuffer;
	// 纹理对象(这里用来保存颜色缓冲)
	unsigned int texColorBuffer;
	// 渲染缓冲(这里用来保存深度缓冲)
	unsigned int rbo;
	// 场景四边形对象
	unsigned int quadVAO;
public:
	void init();
	void render();
	void onDestory();
	std::shared_ptr<Camera> camera;
};

#endif // !SAMPLE9

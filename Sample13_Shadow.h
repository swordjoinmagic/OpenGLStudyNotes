#pragma once
#ifndef SAMPLE13
#define SAMPLE13

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>
#include "Cube.h"
#include <glm/glm.hpp>

class Sample13 {
public:
	void init();
	void render();
	void onDestory();
	std::shared_ptr<Camera> camera;
private:
	void renderScene(Shader *shader);
	// 用于渲染点光源深度图的Shader(即渲染CubeMap)
	std::shared_ptr<Shader> cubeMapDepthShader;
	// 用于渲染平行光深度图的Shader
	std::shared_ptr<Shader> depthShader;
	// 渲染基本物体的Shader
	std::shared_ptr<Shader> objShader;
	// 后处理Shader
	std::shared_ptr<Shader> postEffectShader;
	// 物体的纹理
	std::shared_ptr<Image> objTexture;
	std::shared_ptr<Image> floorTexture;
	std::shared_ptr<Cube> cubeModel;

	// 用于保存点光源深度缓冲的帧缓冲
	GLuint cubeMapDepthMapFBO;
	// 用于保存光源空间深度缓冲的ShadowMap帧缓冲
	GLuint depthMapFBO;
	// 用于保存场景画面的后处理帧缓冲(相当于U3D的RenderTexture)
	unsigned int postEffectFBO;
	// 后处理帧缓冲的颜色缓冲
	unsigned int texColorBuffer;
	// 后处理帧缓冲的深度/模板缓冲
	unsigned int rbo;
	// 屏幕四边形的顶点数组对象
	unsigned int quadVAO;
	// 平行光深度图
	GLuint depthMap;
	// 点光源CubeMap深度图
	GLuint depthCubeMap;
	// 点光源的位置
	std::shared_ptr<glm::vec3> lightPos;
};

#endif // !SAMPLE13

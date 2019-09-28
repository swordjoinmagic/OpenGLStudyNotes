#pragma once
#ifndef SAMPLE19
#define SAMPLE19

#include "Shader.h"
#include "Camera.h"
#include "Image.h"
#include "Model.h"
#include <memory>
#include "Cube.h"
#include <glm/glm.hpp>

class Sample19 {
private:
	std::shared_ptr<Shader> pbrShader;
	std::shared_ptr<Shader> skyBoxShader;

	std::shared_ptr<Model> sphereModel;
	std::shared_ptr<Cube> cubeModel;

	// 用于从hdr中渲染cubeMap的Shader
	std::shared_ptr<Shader> cubeMapShader;

	// 用于从环境贴图cubeMap预处理生成辐照度贴图的Shader
	std::shared_ptr<Shader> irradianceShader;

	// 用于存储从hdr图片
	unsigned int hdrTexture;
	// 从hdr中获得的cubemap图像
	unsigned int hdrCubeMap;
	// 用于渲染cubemap的帧缓冲
	unsigned int captureFBO, captureRBO;
	// 经过预处理后的辐照度CubeMap贴图
	unsigned int irradianceMap;

	void loadHDR();
	void createCubeMapWithHDR();
	void initCubeMapWithHDR();
	void renderSkyBox();
	void createIrradianceMap();
	void renderIrradianceMap();
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE19

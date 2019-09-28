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

	// ���ڴ�hdr����ȾcubeMap��Shader
	std::shared_ptr<Shader> cubeMapShader;

	// ���ڴӻ�����ͼcubeMapԤ�������ɷ��ն���ͼ��Shader
	std::shared_ptr<Shader> irradianceShader;

	// ���ڴ洢��hdrͼƬ
	unsigned int hdrTexture;
	// ��hdr�л�õ�cubemapͼ��
	unsigned int hdrCubeMap;
	// ������Ⱦcubemap��֡����
	unsigned int captureFBO, captureRBO;
	// ����Ԥ�����ķ��ն�CubeMap��ͼ
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

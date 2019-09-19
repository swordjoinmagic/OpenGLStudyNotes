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
	// SSAO��ͼ
	unsigned int ssaoColorBuffer;
	// ���ڴ洢SSAO��ͼ��֡����
	unsigned int ssaoFBO;
	// �洢gBuffer��֡����
	unsigned int gBuffer;
	// ����gBuffer��ͼ����(pos/normal/diffuseColor/depth��)
	unsigned int gPosDepth,gNormal,gAldebo;
	// �����ı��ζ����������
	unsigned int screenQuadVAO;
	// ������ͼ
	unsigned int noiseTexture;
	// ��ȾgBuffer��Shader
	std::shared_ptr<Shader> gShader;
	// ��Ⱦssao��Shader
	std::shared_ptr<Shader> ssaoShader;
	// ��Ⱦ������Shader
	std::shared_ptr<Shader> postEffectShader;
	// ����ģ��
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

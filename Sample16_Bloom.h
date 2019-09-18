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
	// ����֡����
	unsigned int postEffectFBO;
	// ����֡�����е���ɫ����
	unsigned int postEffectColorBuffer[2];
	// ��Ļ�ı��εĶ����������
	unsigned int screenQuadVAO;
	// ���ڸ�˹ģ��������֡����(������˹ģ��,�Ⱥ���������)
	unsigned int blurFBO[2];
	unsigned int blurColorBuffer[2];
	// ����Bloom(��ȡ��ɫ��)Shader
	std::shared_ptr<Shader> bloomShader;
	// ��˹ģ��Shader
	std::shared_ptr<Shader> blurShader;
	// ���ڼ򵥲��Ե�ͼ��
	std::shared_ptr<Image> texture;
	// ����Shader
	std::shared_ptr<Shader> postEffectShader;
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE16

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
	// ������Ⱦ�������shader
	std::shared_ptr<Shader> quadShader;
	// ֡�������(������������(attachment),����Ȼ���/ģ�建��/��ɫ�����)
	unsigned int framebuffer;
	// �������(��������������ɫ����)
	unsigned int texColorBuffer;
	// ��Ⱦ����(��������������Ȼ���)
	unsigned int rbo;
	// �����ı��ζ���
	unsigned int quadVAO;
public:
	void init();
	void render();
	void onDestory();
	std::shared_ptr<Camera> camera;
};

#endif // !SAMPLE9

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
	// �����ı���
	unsigned int screenQuadVAO;
	// ���ڱ���GBuffer��֡����
	unsigned int gBuffer;
	// ���ڱ�����������/����/��������ɫ(��������)
	unsigned int gPosition, gNormal, gAlbedo;
	// ��ȾgBuffer��shader
	std::shared_ptr<Shader> gShader;
	// ��Ⱦ������Shader(�����Ƴٵ�Piexel Shader)
	std::shared_ptr<Shader> deferredShader;
	// ������Shader
	std::shared_ptr<Shader> testShader;
	// ������Ⱦ��Դ��Shader
	std::shared_ptr<Shader> lightBoxShader;
	// ����Ĺ�������
	std::shared_ptr<Image> objTexture;
	// ����ģ��
	std::shared_ptr<Cube> objModel;
	// ���Դλ������ɫ
	std::vector<glm::vec3> lightPositions;
	std::vector<glm::vec3> lightColors;
	// �����������
	std::vector<glm::vec3> objectPositions;
public:
	std::shared_ptr<Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE17

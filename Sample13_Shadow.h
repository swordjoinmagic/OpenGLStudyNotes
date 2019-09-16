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
	// ������Ⱦ���Դ���ͼ��Shader(����ȾCubeMap)
	std::shared_ptr<Shader> cubeMapDepthShader;
	// ������Ⱦƽ�й����ͼ��Shader
	std::shared_ptr<Shader> depthShader;
	// ��Ⱦ���������Shader
	std::shared_ptr<Shader> objShader;
	// ����Shader
	std::shared_ptr<Shader> postEffectShader;
	// ���������
	std::shared_ptr<Image> objTexture;
	std::shared_ptr<Image> floorTexture;
	std::shared_ptr<Cube> cubeModel;

	// ���ڱ�����Դ��Ȼ����֡����
	GLuint cubeMapDepthMapFBO;
	// ���ڱ����Դ�ռ���Ȼ����ShadowMap֡����
	GLuint depthMapFBO;
	// ���ڱ��泡������ĺ���֡����(�൱��U3D��RenderTexture)
	unsigned int postEffectFBO;
	// ����֡�������ɫ����
	unsigned int texColorBuffer;
	// ����֡��������/ģ�建��
	unsigned int rbo;
	// ��Ļ�ı��εĶ����������
	unsigned int quadVAO;
	// ƽ�й����ͼ
	GLuint depthMap;
	// ���ԴCubeMap���ͼ
	GLuint depthCubeMap;
	// ���Դ��λ��
	std::shared_ptr<glm::vec3> lightPos;
};

#endif // !SAMPLE13

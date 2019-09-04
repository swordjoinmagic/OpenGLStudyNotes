#pragma once
#ifndef SAMPLE4
#define SAMPLE4

#include "main.h"
#include <memory>
#include "Shader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Camera.h"

class Sample4 {
private:
	// ��Shader������Ⱦ��Դ
	std::shared_ptr<Shader> whiteShader;
	// ��Shader������Ⱦ�������������
	std::shared_ptr<Shader> lightObjectShader;
	unsigned int VAO;
	unsigned int VBO;

public:
	std::shared_ptr<SJM::Camera> camera;
	void init();
	void render();
	void onDestory();
};

#endif // !SAMPLE4

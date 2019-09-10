#include "Sample8_AlphaBlend.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 grassPosition[] = {
	glm::vec3(0,0,0),
	glm::vec3(1,0,1),
	glm::vec3(-1,0,-1)
};

const glm::vec3 objectPosition[] = {
	glm::vec3(1,0,0),
	glm::vec3(3,0,0),
	glm::vec3(5,0,0)
};

void Sample8::init() {
	// 初始化模型
	quad = std::make_shared<Model>("mesh/quad.obj");
	// 初始化Shader和纹理
	shader = std::make_shared<Shader>("Shader/Sample8 AlphaBlend/vertex.glsl", "Shader/Sample8 AlphaBlend/frag.glsl");
	alphaTextTexture = std::make_shared<Image>("Image/grass.png");
	alphaBlendTexture = std::make_shared<Image>("Image/blending_transparent_window.png");
}

float angle = 0;

void Sample8::render() {
	shader->use();

	// 设置mvp矩阵
	shader->setMatrix4x4("view",glm::value_ptr(camera->getViewMatrix()));
	shader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));
	shader->setTexture2D("MainTex",*alphaTextTexture,0);

	for (int i = 0; i < grassPosition->length();i++) {
		glm::mat4 model(1.0f);
		model = glm::translate(model,grassPosition[i]);
		shader->setMatrix4x4("model",glm::value_ptr(model));

		quad->Draw(*shader);
	}

	// 开启AlphaBlend
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA,GL_ONE_MINUS_SRC_ALPHA);

	shader->setTexture2D("MainTex",*alphaBlendTexture,0);
	for (int i = 0; i < objectPosition->length(); i++) {
		glm::mat4 model(1.0f);
		model = glm::translate(model, objectPosition[i]);
		shader->setMatrix4x4("model", glm::value_ptr(model));

		quad->Draw(*shader);
	}

	// 恢复默认状态
	glDisable(GL_BLEND);
}

void Sample8::onDestory() {

}
#include "Sample6_Depth.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 cubePosition[] = {
	glm::vec3(0,0,0) * 3.0f,
	glm::vec3(1,0,1) * 3.0f,
	glm::vec3(0,1,2) * 3.0f,
	glm::vec3(-1,-1,2) * 3.0f,
};

void Sample6::init() {
	// 初始化Cube
	cube = std::make_shared<Model>("mesh/suzanne.obj");

	// 初始化Shader
	shader = std::make_shared<Shader>("Shader/Sample6 Depth/showDepthV.glsl", "Shader/Sample6 Depth/showDepthF.glsl");
}

void Sample6::render() {
	shader->use();

	// 设置VP矩阵
	shader->setMatrix4x4("view",glm::value_ptr(camera->getViewMatrix()));
	shader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));

	// 创建N个立方体,查看他们的深度
	for (unsigned int i = 0; i < cubePosition->length();i++) {
		glm::mat4 mMatrix(1.0f);
		mMatrix = glm::translate(mMatrix,cubePosition[i]);
		shader->setMatrix4x4("model",glm::value_ptr(mMatrix));

		cube->Draw(*shader);
	}
}

void Sample6::onDestory() {

}
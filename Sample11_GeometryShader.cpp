#include "Sample11_GeometryShader.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
const float points[] = {
	-0.5f,  0.5f, // 左上
	 0.5f,  0.5f, // 右上
	 0.5f, -0.5f, // 右下
	-0.5f, -0.5f  // 左下
};

void Sample11::init() {

	explodeShader = std::make_shared<Shader>("Shader/Sample11 Geometry/explodeV.glsl", "Shader/Sample11 Geometry/explodeF.glsl", "Shader/Sample11 Geometry/explodeG.glsl");
	explodeModel = std::make_shared<Model>("mesh/nanosuit/nanosuit.obj");
	explodeTexture = std::make_shared<Image>("Image/wall.jpg");

	shader = std::make_shared<Shader>("Shader/Sample11 Geometry/v.glsl", "Shader/Sample11 Geometry/f.glsl", "Shader/Sample11 Geometry/g.glsl");

	unsigned int VBO;
	// 创建顶点缓冲对象并绑定
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(points),&points,GL_STATIC_DRAW);

	// 设置并启用顶点属性
	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,2,GL_FLOAT,GL_FALSE,2*sizeof(float),0);

	// 恢复默认绑定
	glBindVertexArray(0);
}

void Sample11::render() {

	glEnable(GL_DEPTH_TEST);

	glm::mat4 model(1.0);
	explodeShader->use();
	explodeShader->setMatrix4x4("model",glm::value_ptr(model));
	explodeShader->setMatrix4x4("view",glm::value_ptr(camera->getViewMatrix()));
	explodeShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));
	explodeShader->setFloat("time",glfwGetTime());
	explodeShader->setTexture2D("MainTex",*explodeTexture,0);

	explodeModel->Draw(*explodeShader);
}

void Sample11::onDestory() {}
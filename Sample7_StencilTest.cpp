#include "Sample7_StencilTest.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 objectPosition[] = {
	glm::vec3(0,0,0),
	//glm::vec3(1,1,1)
};

void Sample7::init() {
	// 初始化Shader
	cubeShader = std::make_shared<Shader>("Shader/Sample7 StenncilTest/vertex.glsl", "Shader/Sample7 StenncilTest/cubeShader.glsl");
	outLineShader = std::make_shared<Shader>("Shader/Sample7 StenncilTest/vertex.glsl", "Shader/Sample7 StenncilTest/outlineShader.glsl");

	// 初始化纹理
	texture = std::make_shared<Image>("Image/container.jpg");

	// 初始化模型
	model = std::make_shared<Cube>();
	
}

void Sample7::render() {
	// 设定通过模板测试时,将模板值设为1
	// 且物体将永远通过模板测试
	glStencilFunc(GL_ALWAYS, 1.0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// 启用深度测试,模板测试,先正常绘制箱子
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);

	cubeShader->use();	
	// 设置vp矩阵
	cubeShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	cubeShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));

	// 设置纹理
	cubeShader->setTexture2D("MainTex",*texture,0);

	for (int i = 0; i < objectPosition->length(); i++) {
		glm::mat4 mMatrix(1.0f);
		mMatrix = glm::translate(mMatrix,objectPosition[i]);
		cubeShader->setMatrix4x4("model",glm::value_ptr(mMatrix));
		model->Draw(*cubeShader);
	}

	// 关闭深度测试,关闭模板写入
	// 并设置当当前片元的模板值不等于1的时候,不进行渲染
	glStencilFunc(GL_NOTEQUAL,1,0xFF);
	glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);

	// 将物体缩放一点,绘制它的轮廓线
	outLineShader->use();
	// 设置vp矩阵
	outLineShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	outLineShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));

	for (int i = 0; i < objectPosition->length(); i++) {
		// 将物体放大一点
		glm::mat4 mMatrix(1.0f);
		mMatrix = glm::scale(mMatrix,glm::vec3(1,1,1)*1.1f);
		mMatrix = glm::translate(mMatrix, objectPosition[i]);

		outLineShader->setMatrix4x4("model",glm::value_ptr(mMatrix));

		model->Draw(*outLineShader);
	}


	// 恢复默认设置
	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xFF);
}

void Sample7::onDestory() {

}

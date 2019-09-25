#include "Sample7_StencilTest.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 objectPosition[] = {
	glm::vec3(0,0,0),
	//glm::vec3(1,1,1)
};

void Sample7::init() {
	// ��ʼ��Shader
	cubeShader = std::make_shared<Shader>("Shader/Sample7 StenncilTest/vertex.glsl", "Shader/Sample7 StenncilTest/cubeShader.glsl");
	outLineShader = std::make_shared<Shader>("Shader/Sample7 StenncilTest/vertex.glsl", "Shader/Sample7 StenncilTest/outlineShader.glsl");

	// ��ʼ������
	texture = std::make_shared<Image>("Image/container.jpg");

	// ��ʼ��ģ��
	model = std::make_shared<Cube>();
	
}

void Sample7::render() {
	// �趨ͨ��ģ�����ʱ,��ģ��ֵ��Ϊ1
	// �����彫��Զͨ��ģ�����
	glStencilFunc(GL_ALWAYS, 1.0, 0xFF);
	glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

	// ������Ȳ���,ģ�����,��������������
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_STENCIL_TEST);
	glStencilMask(0xFF);

	cubeShader->use();	
	// ����vp����
	cubeShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	cubeShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));

	// ��������
	cubeShader->setTexture2D("MainTex",*texture,0);

	for (int i = 0; i < objectPosition->length(); i++) {
		glm::mat4 mMatrix(1.0f);
		mMatrix = glm::translate(mMatrix,objectPosition[i]);
		cubeShader->setMatrix4x4("model",glm::value_ptr(mMatrix));
		model->Draw(*cubeShader);
	}

	// �ر���Ȳ���,�ر�ģ��д��
	// �����õ���ǰƬԪ��ģ��ֵ������1��ʱ��,��������Ⱦ
	glStencilFunc(GL_NOTEQUAL,1,0xFF);
	glStencilMask(0x00);
	//glDisable(GL_DEPTH_TEST);

	// ����������һ��,��������������
	outLineShader->use();
	// ����vp����
	outLineShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	outLineShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));

	for (int i = 0; i < objectPosition->length(); i++) {
		// ������Ŵ�һ��
		glm::mat4 mMatrix(1.0f);
		mMatrix = glm::scale(mMatrix,glm::vec3(1,1,1)*1.1f);
		mMatrix = glm::translate(mMatrix, objectPosition[i]);

		outLineShader->setMatrix4x4("model",glm::value_ptr(mMatrix));

		model->Draw(*outLineShader);
	}


	// �ָ�Ĭ������
	glEnable(GL_DEPTH_TEST);
	glStencilMask(0xFF);
}

void Sample7::onDestory() {

}

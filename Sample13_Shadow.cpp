#include "Sample13_Shadow.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT=600;


// ��Ļ�ı��εĶ�������
const float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions		 // texCoords
	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

void Sample13::init() {

#pragma region ��ʼ�����Դ���ͼ֡����(CubeMap)

	// ��ʼ����Դλ��
	lightPos = std::make_shared<glm::vec3>(0,1.5f,0);

	// ��ʼ����������ͼ
	glGenTextures(1,&depthCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP,depthCubeMap);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(
			GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,
			0,GL_DEPTH_COMPONENT,
			SHADOW_WIDTH,SHADOW_HEIGHT,
			0,GL_DEPTH_COMPONENT,
			GL_FLOAT,NULL);
	}
	// ����cubeMap�Ĳ����͹���ģʽ
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// ��ʼ��֡����
	glGenFramebuffers(1,&cubeMapDepthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,cubeMapDepthMapFBO);

	// �����Ȼ��帽��
	glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depthCubeMap,0);
	// ��ʹ����ɫ���������Ⱦ
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// ���֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);

#pragma endregion

#pragma region ƽ�й����ͼ֡����

	// Ϊ��Ⱦ�������ͼ����һ��֡�������
	glGenFramebuffers(1,&depthMapFBO);

	// ����һ��2D����,��֡�������Ȼ���ʹ��
	glGenTextures(1,&depthMap);
	glBindTexture(GL_TEXTURE_2D,depthMap);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	// ���ò����Լ�����ģʽ
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = {1.0,1.0,1.0,1.0};
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

	// �����ɵ����������Ϊ֡�������Ȼ���
	glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthMap,0);
	// ��ʽ����GL���ǲ�ʹ���κ���ɫ���ݽ�����Ⱦ
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// ���֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);

#pragma endregion

#pragma region ����֡����

	// ���ɺ���֡�������
	glGenFramebuffers(1,&postEffectFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,postEffectFBO);

	// ���ɲ�������(������ɫ����)
	glGenTextures(1,&texColorBuffer);
	glBindTexture(GL_TEXTURE_2D,texColorBuffer);
	// ����һ���յ�����,����Ļ���һ��
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,SCREEN_WIDTH,SCREEN_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	// �����������ģʽ
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	// �������
	glBindTexture(GL_TEXTURE_2D,0);

	// �������ɵ���������Ը���(attachment)����ʽ��ӵ���ǰ֡���������
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texColorBuffer,0);


	// ������Ⱦ�������(ֻ�ɶ�����д),�������/ģ�建��
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	// ���ø�ʽ
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,SCREEN_WIDTH,SCREEN_HEIGHT);

	// ����Ⱦ������󸽼ӵ���ǰ֡�������
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);

	// ���֡�����Ƿ�����,���������,��ӡ������Ϣ
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "֡���岻����!!!" << std::endl;
	}

	// ���֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);
#pragma endregion

	// ��ʼ����Ļ�ı��εĶ�������
	unsigned int quadVBO;
	glGenVertexArrays(1,&quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1,&quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);

	// ���ò����ö�������

	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);
	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

	// ���VBO,VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	// ��ʼ��Model
	cubeModel = std::make_shared<Cube>();
	// ��ʼ��Shader
	depthShader = std::make_shared<Shader>("Shader/Sample13 Shadow/depthV.glsl", "Shader/Sample13 Shadow/depthF.glsl");
	objShader = std::make_shared<Shader>("Shader/Sample13 Shadow/v.glsl", "Shader/Sample13 Shadow/f.glsl");
	objTexture = std::make_shared<Image>("Image/container.jpg");
	floorTexture = std::make_shared<Image>("Image/wall.jpg");
	postEffectShader = std::make_shared<Shader>("Shader/Sample13 Shadow/postEffectV.glsl", "Shader/Sample13 Shadow/postEffectF.glsl");
	
	cubeMapDepthShader = std::make_shared<Shader>("Shader/Sample13 Shadow/pointShadowCreateCubeMapV.glsl", "Shader/Sample13 Shadow/pointShadowCreateCubeMapF.glsl", "Shader/Sample13 Shadow/pointShadowCreateCubeMapG.glsl");
}

void Sample13::render() {

#pragma region ��Ⱦ���Դ���ͼ

	// ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER,cubeMapDepthMapFBO);
	// �����ӿ�
	glViewport(0,0,SHADOW_WIDTH,SHADOW_HEIGHT);
	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	// ��ո�֡�������Ȼ���
	glClear(GL_DEPTH_BUFFER_BIT);

	// ���ù�Դ�ռ��ͶӰ����
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat near1 = 1.0f;
	GLfloat far1 = 25.0f;
	glm::mat4 pointShaderProj = glm::perspective(glm::radians(90.0f),aspect,near1,far1);

	// ���ù۲����(6�����)
	std::vector<glm::mat4> pointShadowTransforms;
	pointShadowTransforms.push_back(
		pointShaderProj *
		glm::lookAt(*lightPos, *lightPos + glm::vec3(1, 0, 0), glm::vec3(0, -1, 0))
	);

	pointShadowTransforms.push_back(
		pointShaderProj *
		glm::lookAt(*lightPos, *lightPos + glm::vec3(-1, 0, 0), glm::vec3(0, -1, 0))
	);

	pointShadowTransforms.push_back(
		pointShaderProj *
		glm::lookAt(*lightPos, *lightPos + glm::vec3(0, 1, 0), glm::vec3(0, 0, 1))
	);

	pointShadowTransforms.push_back(
		pointShaderProj *
		glm::lookAt(*lightPos, *lightPos + glm::vec3(0, -1, 0), glm::vec3(0, 0, -1))
	);

	pointShadowTransforms.push_back(
		pointShaderProj *
		glm::lookAt(*lightPos, *lightPos + glm::vec3(0, 0, 1), glm::vec3(0, -1, 0))
	);

	pointShadowTransforms.push_back(
		pointShaderProj *
		glm::lookAt(*lightPos, *lightPos + glm::vec3(0, 0, -1), glm::vec3(0, -1, 0))
	);

	cubeMapDepthShader->use();
	cubeMapDepthShader->setFloat3("lightPos",lightPos->x, lightPos->y, lightPos->z);
	cubeMapDepthShader->setFloat("far_plane",far1);
	// ���ù�Դ�ռ�vp����
	for (unsigned int i = 0; i < 6; i++) {
		cubeMapDepthShader->setMatrix4x4("shadowMatrices["+std::to_string(i)+"]",glm::value_ptr(pointShadowTransforms[i]));
	}
	renderScene(cubeMapDepthShader.get());

	// �ָ�Ĭ���ӿ�
	glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	// ���֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);

#pragma endregion

#pragma region ��Ⱦƽ�й����ͼ

	// �޳�����
	glCullFace(GL_FRONT);

	// �Ȱ���Ⱦ���ͼ��֡������Ⱦһ��,�Եõ���Դ�ռ�����ͼ
	glViewport(0,0,SHADOW_WIDTH,SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// ������Դ�ռ����ͼ��ͶӰ����
	float near_plane = 1.0f,far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f,4.0f,-1.0f),glm::vec3(0.0f),glm::vec3(1.0f));

	depthShader->use();
	depthShader->setMatrix4x4("view", glm::value_ptr(lightView));
	depthShader->setMatrix4x4("projection", glm::value_ptr(lightProjection));

	renderScene(depthShader.get());

	// �ָ��޳�
	glCullFace(GL_BACK);
#pragma endregion

#pragma region ��Ⱦ����֡����(��������������Ⱦ��һ��RenderTexture��)

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// �󶨺���֡����
	glBindFramebuffer(GL_FRAMEBUFFER,postEffectFBO);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	objShader->use();
	objShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	objShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	objShader->setMatrix4x4("lightView",glm::value_ptr(lightView));
	objShader->setMatrix4x4("lightProjection",glm::value_ptr(lightProjection));	
	// ����ƽ�й������ͼ
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,depthMap);
	objShader->setInt("depthMap",1);

	// ���õ��Դ�����ͼ
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP,depthCubeMap);
	objShader->setInt("pointDepthMap",2);
	// ���õ��Դλ��
	objShader->setFloat3("lightPos",lightPos->x, lightPos->y, lightPos->z);
	// ���ù�Դ�ռ�Զƽ��
	objShader->setFloat("far_plane",far1);

	renderScene(objShader.get());


#pragma endregion

#pragma region ������֡������Ⱦ����Ļ�ı�����

	// ��Ĭ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// �ر���Ȳ���
	glDisable(GL_DEPTH_TEST);

	postEffectShader->use();
	glBindVertexArray(quadVAO);
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	postEffectShader->setInt("screenTex",0);

	glDrawArrays(GL_TRIANGLES,0,6);

#pragma endregion

}


void Sample13::renderScene(Shader* shader) {
	//=====================
	// ���Ƶذ�

	shader->use();

	// �ذ��Model
	glm::mat4 floormMatrix(1.0f);
	floormMatrix = glm::translate(floormMatrix, glm::vec3(0, -0.5f, 0));
	floormMatrix = glm::scale(floormMatrix, glm::vec3(25, 0.1f, 25));
	// ����mvp����
	shader->setMatrix4x4("model", glm::value_ptr(floormMatrix));
	// ��������
	shader->setTexture2D("MainTex", *floorTexture, 0);

	// ��������
	cubeModel->Draw(*shader);

	//========================
	// ����������

	shader->use();
	shader->setTexture2D("MainTex", *objTexture, 0);

	// �������Model
	glm::mat4 mMatrix(1.0f);
	// ��������������
	mMatrix = glm::translate(mMatrix, glm::vec3(0.0f, 1.5f, 0.0f));
	shader->setMatrix4x4("model", glm::value_ptr(mMatrix));
	cubeModel->Draw(*shader);

	mMatrix = glm::mat4(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(2.0f, 0.0f, 1.0f));
	shader->setMatrix4x4("model", glm::value_ptr(mMatrix));
	cubeModel->Draw(*shader);

	mMatrix = glm::mat4(1.0f);
	mMatrix = glm::translate(mMatrix, glm::vec3(-1.0f, 0.0f, 2.0f));
	mMatrix = glm::rotate(mMatrix, 60.0f, glm::normalize(glm::vec3(1.0, 0.0, 1.0)));
	mMatrix = glm::scale(mMatrix, glm::vec3(0.5f));
	shader->setMatrix4x4("model", glm::value_ptr(mMatrix));
	cubeModel->Draw(*shader);
}

void Sample13::onDestory() {}

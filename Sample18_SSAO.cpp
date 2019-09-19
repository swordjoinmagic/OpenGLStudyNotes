#include "Sample18_SSAO.h"	
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <random> 

GLfloat lerp(GLfloat a,GLfloat b,GLfloat f) {
	return a + f * (b - a);
}

const float screenQuadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions		 // texCoords
	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};


void Sample18::init() {

#pragma region ��ʼ�������ı���

	unsigned int screenQuadVBO;
	// ��ʼ����Ļ�ı���
	glGenVertexArrays(1, &screenQuadVAO);
	glBindVertexArray(screenQuadVAO);
	glGenBuffers(1, &screenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER, screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(screenQuadVertices), screenQuadVertices, GL_STATIC_DRAW);

	// ���ò����ö�������
	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), 0);
	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));

	// ���vao,vbo
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

#pragma endregion

#pragma region ��ʼ�����ڴ洢GBuffer��֡����
	glGenFramebuffers(1, &gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);

	// pos+depth Buffer
	glGenTextures(1, &gPosDepth);
	glBindTexture(GL_TEXTURE_2D, gPosDepth);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGBA, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	// ��������Ϊ�������뵽֡����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, gPosDepth, 0);

	// normal Buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// ��������Ϊ�������뵽֡����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// diffuse specular Buffer(rgb����������ɫ,a��߹ⷴ���gloss��)
	glGenTextures(1, &gAldebo);
	glBindTexture(GL_TEXTURE_2D, gAldebo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// ��������Ϊ�������뵽֡����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAldebo, 0);

	// ����OpenGL���ǽ�ʹ��3����ɫ����(ʹ��Shader�п��������ɫ�����Ŀ��)
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// ������Ȼ���
	GLuint rboDepth;
	glGenRenderbuffers(1, &rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, 800, 600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);

	// ���֡�����Ƿ�����,���������,��ӡ������Ϣ
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "֡���岻����!!!" << std::endl;
	}

	// ���֡����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

#pragma endregion

#pragma region ��ʼ�����ڴ洢SSAO��ͼ��֡����

	glGenFramebuffers(1,&ssaoFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,ssaoFBO);

	// ��ʼ��ssao��ͼ(��ɫ����)
	glGenTextures(1,&ssaoColorBuffer);
	glBindTexture(GL_TEXTURE_2D,ssaoColorBuffer);
	// ssao��ͼ�����һ���Ҷ�ֵ,����ֻ��Ҫr����
	glTexImage2D(GL_TEXTURE_2D,0,GL_RED,800,600,0,GL_RGB,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// ����ɫ������ӽ�֡����
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,ssaoColorBuffer,0);

	// ���֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);
#pragma endregion


	// ��ʼ��ģ��
	cubeModel = std::make_shared<Cube>();
	objModel = std::make_shared<Model>("mesh/nanosuit/nanosuit.obj");

	// ��ʼ��shader
	gShader = std::make_shared<Shader>("Shader/Sample18 SSAO/gShaderV.glsl", "Shader/Sample18 SSAO/gShaderF.glsl");
	postEffectShader = std::make_shared<Shader>("Shader/Sample17 DeferredShading/testV.glsl", "Shader/Sample18 SSAO/postEffectF.glsl");
	ssaoShader = std::make_shared<Shader>("Shader/Sample17 DeferredShading/testV.glsl", "Shader/Sample18 SSAO/ssaoShaderF.glsl");

	// �������һ��ӵ�����64�������Ĳ�������(����ռ�Ϊ���߿ռ���,(0,0,1)Ϊ����������)
	std::uniform_real_distribution<GLfloat> randomFloats(0.0, 1.0); // generates random floats between 0.0 and 1.0
	std::default_random_engine generator;
	for (unsigned int i = 0; i < 64;i++) {
		glm::vec3 sample(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, randomFloats(generator));
		sample = glm::normalize(sample);
		sample *= randomFloats(generator);
		GLfloat scale = GLfloat(i) / 64.0;

		// Scale samples s.t. they're more aligned to center of kernel
		scale = lerp(0.1f, 1.0f, scale * scale);
		sample *= scale;
		ssaoKernel.push_back(sample);
	}

	// ��ʼ��������ͼ
	std::vector<glm::vec3> ssaoNoise;
	for (GLuint i = 0; i < 16;i++) {
		glm::vec3 noise(randomFloats(generator) * 2.0 - 1.0, randomFloats(generator) * 2.0 - 1.0, 0.0f);
		ssaoNoise.push_back(noise);
	}
	glGenTextures(1,&noiseTexture);
	glBindTexture(GL_TEXTURE_2D,noiseTexture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 4, 4, 0, GL_RGB, GL_FLOAT, &ssaoNoise[0]);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

}

void Sample18::render() {

#pragma region ��ȾGBuffer
	// ����Pass
	// �Ƚ�������Ⱦ��GBuffer��
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	gShader->use();
	// ����vp����
	gShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	gShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));

	// �ֱ���Ⱦ�����Plane
	glm::mat4 model(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 5.0));
	model = glm::rotate(model, 80.0f, glm::vec3(1.0, 0.0, 0.0));
	model = glm::scale(model, glm::vec3(0.5f));

	gShader->setMatrix4x4("model", glm::value_ptr(model));
	gShader->setFloat("near", camera->nearDistance);
	gShader->setFloat("far", camera->farDistance);

	objModel->Draw(*gShader);

	model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0, -1.5f, 0.0f));
	model = glm::scale(model, glm::vec3(20.0f, 1.0f, 20.0f));
	gShader->setMatrix4x4("model", glm::value_ptr(model));

	cubeModel->Draw(*gShader);
#pragma endregion

#pragma region ��ȾSSAO��ͼ

	// ʹ��g������Ⱦssao����
	glBindFramebuffer(GL_FRAMEBUFFER,ssaoFBO);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	ssaoShader->use();

	// ����gpos/gnormal/noise����
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosDepth);
	ssaoShader->setInt("gPosDepth",0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	ssaoShader->setInt("gNormal", 1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, noiseTexture);
	ssaoShader->setInt("NoiseTex", 2);

	ssaoShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));
	// ���������������
	for (unsigned int i = 0; i < ssaoKernel.size(); i++) {
		ssaoShader->setFloat3("samples["+std::to_string(i)+"]",ssaoKernel[i].x, ssaoKernel[i].y, ssaoKernel[i].z);
	}

	glDisable(GL_DEPTH_TEST);
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ���
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glBindTexture(GL_TEXTURE_2D,0);
#pragma endregion



#pragma region ��Ⱦ����
	// ��դ��Pass
	// ����gbuffer��Ⱦ�����ı���
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glDisable(GL_DEPTH_TEST);

	// ���ø���GBuffer
	postEffectShader->use();
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gPosDepth);
	postEffectShader->setInt("gPosDepth",0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,gNormal);
	postEffectShader->setInt("gNormal",1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAldebo);
	postEffectShader->setInt("gAlbedo", 2);
	glActiveTexture(GL_TEXTURE3);
	glBindTexture(GL_TEXTURE_2D, ssaoColorBuffer);
	postEffectShader->setInt("ssao", 3);
	// ���ù��ղ���
	glm::vec3 lightPos = glm::vec3(2.0, 4.0, -2.0);
	glm::vec3 lightColor = glm::vec3(0.2, 0.2, 0.7);
	// �۲�ռ�Ĺ�Դλ��
	glm::vec3 lightPosView = glm::vec3(camera->getViewMatrix() * glm::vec4(lightPos,1.0));

	const GLfloat constant = 1.0;
	const GLfloat linear = 0.09;
	const GLfloat quadratic = 0.032;
	postEffectShader->setFloat3("light.pos",lightPosView.x, lightPosView.y, lightPosView.z);
	postEffectShader->setFloat3("light.color",lightColor.x, lightColor.y, lightColor.z);
	postEffectShader->setFloat("light.linear",linear);
	postEffectShader->setFloat("light.quadratic",quadratic);

	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion

}

void Sample18::onDestory() {}
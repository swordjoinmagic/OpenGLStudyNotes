#include "Sample17_DeferredShading.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float screenQuadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions		 // texCoords
	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

// һ��32�����Դ
const unsigned int NR_LIGHTS = 32;

const GLfloat constant = 1.0; // Note that we don't send this to the shader, we assume it is always 1.0 (in our case)
const GLfloat linear = 0.7;
const GLfloat quadratic = 1.8;

void Sample17::init() {

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


#pragma region ��ʼ����������
	objectPositions.push_back(glm::vec3(-3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, -3.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 0.0));
	objectPositions.push_back(glm::vec3(-3.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(0.0, -3.0, 3.0));
	objectPositions.push_back(glm::vec3(3.0, -3.0, 3.0));
#pragma endregion


#pragma region ��ʼ�����Դ

	srand(13);
	for (unsigned int i = 0; i < NR_LIGHTS;i++) {
		GLfloat xpos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		GLfloat ypos = ((rand() % 100) / 100.0) * 6.0 - 4.0;
		GLfloat zpos = ((rand() % 100) / 100.0) * 6.0 - 3.0;
		lightPositions.push_back(glm::vec3(xpos,ypos,zpos));
		GLfloat rColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat gColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		GLfloat bColor = ((rand() % 100) / 200.0f) + 0.5; // Between 0.5 and 1.0
		lightColors.push_back(glm::vec3(rColor, gColor, bColor));
	}

#pragma endregion

#pragma region ��ʼ�����ڴ洢GBuffer��֡����
	glGenFramebuffers(1,&gBuffer);
	glBindFramebuffer(GL_FRAMEBUFFER,gBuffer);

	// pos Buffer
	glGenTextures(1,&gPosition);
	glBindTexture(GL_TEXTURE_2D,gPosition);
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,800,600,0,GL_RGB,GL_FLOAT,NULL);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

	// ��������Ϊ�������뵽֡����
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,gPosition,0);

	// normal Buffer
	glGenTextures(1, &gNormal);
	glBindTexture(GL_TEXTURE_2D, gNormal);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB16F, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// ��������Ϊ�������뵽֡����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT1, GL_TEXTURE_2D, gNormal, 0);

	// diffuse specular Buffer(rgb����������ɫ,a��߹ⷴ���gloss��)
	glGenTextures(1, &gAlbedo);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_FLOAT, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	// ��������Ϊ�������뵽֡����
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT2, GL_TEXTURE_2D, gAlbedo, 0);

	// ����OpenGL���ǽ�ʹ��3����ɫ����(ʹ��Shader�п��������ɫ�����Ŀ��)
	GLuint attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glDrawBuffers(3, attachments);

	// ������Ȼ���
	GLuint rboDepth;
	glGenRenderbuffers(1,&rboDepth);
	glBindRenderbuffer(GL_RENDERBUFFER,rboDepth);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT,800,600);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,rboDepth);

	// ���֡�����Ƿ�����,���������,��ӡ������Ϣ
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "֡���岻����!!!" << std::endl;
	}

	// ���֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);

#pragma endregion

	// ��ʼ��ģ��
	objModel = std::make_shared<Cube>();
	// ��ʼ����ȾShader
	gShader = std::make_shared<Shader>("Shader/Sample17 DeferredShading/gShaderV.glsl", "Shader/Sample17 DeferredShading/gShaderF.glsl");
	testShader = std::make_shared<Shader>("Shader/Sample17 DeferredShading/testV.glsl", "Shader/Sample17 DeferredShading/testF.glsl");
	deferredShader = std::make_shared<Shader>("Shader/Sample17 DeferredShading/deferredShaderV.glsl", "Shader/Sample17 DeferredShading/deferredShaderF.glsl");
	// ��ʼ������
	objTexture = std::make_shared<Image>("Image/Rock-Texture-Surface.jpg");

	lightBoxShader = std::make_shared<Shader>("Shader/Sample17 DeferredShading/lightBoxV.glsl", "Shader/Sample17 DeferredShading/lightBoxF.glsl");
}

void Sample17::render() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	// ����ȾGBuffer,�Ƴ�������ƬԪ��ɫ����ִ��
	glBindFramebuffer(GL_FRAMEBUFFER,gBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	gShader->use();
	// ����vp����
	gShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	gShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));

	// ��������
	gShader->setTexture2D("diffuseTex",*objTexture,0);

	for (int i = 0; i < objectPositions.size();i++) {
		glm::vec3 pos = objectPositions[i];
		glm::mat4 model(1.0);
		model = glm::translate(model,pos);
		gShader->setMatrix4x4("model",glm::value_ptr(model));

		// ��Ⱦ����������Ե�һϵ��gBuffer��
		objModel->Draw(*gShader);
	}
	
	// ��Ĭ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	// �ر���Ȳ���
	glDisable(GL_DEPTH_TEST);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	deferredShader->use();
	
	// ����gBuffer
	glBindTexture(GL_TEXTURE_2D,gPosition);
	deferredShader->setInt("gPos",0);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,gNormal);
	deferredShader->setInt("gNormal",1);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, gAlbedo);
	deferredShader->setInt("gAldebo", 2);
	
	// ���ù۲�λ��
	deferredShader->setFloat3("viewPos",camera->pos.x, camera->pos.y, camera->pos.z);

	// ����ÿ����Դ�Ĳ���
	for (int i = 0; i < NR_LIGHTS; i++) {
		// lightPos
		deferredShader->setFloat3("lights["+std::to_string(i)+"].position",lightPositions[i].x, lightPositions[i].y, lightPositions[i].z);
		// lightColor
		deferredShader->setFloat3("lights[" + std::to_string(i) + "].color", lightColors[i].x, lightColors[i].y, lightColors[i].z);

		// ˥������
		deferredShader->setFloat("lights[" + std::to_string(i) + "].linear", linear);
		deferredShader->setFloat("lights[" + std::to_string(i) + "].quadratic", quadratic);
	}

	// ��Ⱦ�����ı���
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES,0,6);

	// ��gbuffer�е���Ȼ��帴�Ƶ�Ĭ�ϻ�����(��ʵ����ǰ����Ⱦ���)
	glBindFramebuffer(GL_READ_FRAMEBUFFER,gBuffer);
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER,0);
	glBlitFramebuffer(0,0,800,600,0,0,800,600,GL_DEPTH_BUFFER_BIT,GL_NEAREST);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	glEnable(GL_DEPTH_TEST);

	// ǰ����Ⱦ(��ǰ����ӳ���Ⱦ����)
	lightBoxShader->use();
	lightBoxShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	lightBoxShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	for (unsigned int i = 0; i < lightPositions.size();i++) {
		glm::mat4 model(1.0f);
		model = glm::translate(model,lightPositions[i]);
		model = glm::scale(model,glm::vec3(0.2f,0.2f,0.2f));
		lightBoxShader->setMatrix4x4("model",glm::value_ptr(model));

		lightBoxShader->setFloat3("lightColor",lightColors[i].r, lightColors[i].g, lightColors[i].b);

		objModel->Draw(*lightBoxShader);
	}

}

void Sample17::onDestory() {}
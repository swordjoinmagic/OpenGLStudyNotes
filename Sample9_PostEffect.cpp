#include "Sample9_PostEffect.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const glm::vec3 objectPosotion[] = {
	glm::vec3(0,0,0),
	glm::vec3(1,1,1),
	glm::vec3(-1,-1,-1),
};

const float quadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions		 // texCoords
	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

void Sample9::init() {

	unsigned int quadVBO;
	// ��ʼ�������ı��ζ���
	glGenVertexArrays(1,&quadVAO);
	glGenBuffers(1,&quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),&quadVertices,GL_STATIC_DRAW);

	// ���ö�������

	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);

	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

	// �ָ�Ĭ�ϰ󶨶���
	glBindVertexArray(0);


	// ��ʼ��ģ�Ͷ���
	obj = std::make_shared<Model>("mesh/quad.obj");

	// ��ʼ��shader������
	objShader = std::make_shared<Shader>("Shader/Sample9 PostEffect/objVertex.glsl", "Shader/Sample9 PostEffect/objFrag.glsl");
	objTexture = std::make_shared<Image>("Image/wall.jpg");

	quadShader = std::make_shared<Shader>("Shader/Sample9 PostEffect/postEffectVertex.glsl", "Shader/Sample9 PostEffect/postEffectFrag.glsl");

	// ����һ��֡������󲢰���
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// ��������(������ɫ����)
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	// ����һ���յ�����,��С����Ļ���һ��
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// �����������ģʽΪ���Բ���
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// �ָ�Ĭ�ϵİ�����
	glBindTexture(GL_TEXTURE_2D, 0);

	// ����ǰ������󸽼ӵ���ǰ��֡�������
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// ������Ⱦ�������
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	
	// ����Ⱦ������������ǰ֡�������
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,rbo);

	// ���֡�����Ƿ�����,���������,��ӡ������Ϣ
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
		std::cout << "֡���岻����!!!" << std::endl;
	}

	// �ָ�Ĭ��״̬
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Sample9::render() {
	// �����½���֡����
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glClearColor(0.1f,0.1f,0.1f,1.0f);
	// �����ɫ�������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	objShader->use();
	objShader->setMatrix4x4("view",glm::value_ptr(camera->getViewMatrix()));
	objShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));

	objShader->setTexture2D("MainTex",*objTexture,0);

	// ���Ƴ�����֡������ȥ
	for (int i = 0; i < objectPosotion->length(); i++) {
		glm::mat4 model(1.0f);
		model = glm::translate(model,objectPosotion[i]);
		objShader->setMatrix4x4("model",glm::value_ptr(model));

		obj->Draw(*objShader);
	}

	// �ڶ������ƽ׶�,���½�֡�����е���ɫ����(����)��Ⱦ
	// �������е�һ���ı�����(quad)
	
	// ���Ȱ�Ĭ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// ʹ�ú���Shader����������Ļ������
	quadShader->use();
	// ����Ļ�ı��εĶ����������
	glBindVertexArray(quadVAO);
	// �ر���Ȳ���
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D,texColorBuffer);
	quadShader->setInt("screenTex",0);

	glDrawArrays(GL_TRIANGLES,0,6);

}

void Sample9::onDestory() {}
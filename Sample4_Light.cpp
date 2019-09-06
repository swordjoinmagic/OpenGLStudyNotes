#include "Sample4_Light.h"

// ��Դλ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
// ��Դ��ɫ
glm::vec4 lightColor(1,1,1,1);

void Sample4::init() {

	// ��ʼ���������
	image = std::make_shared<SJM::Image>("Image/container.jpg");

	// ��ʼ����Ⱦ��Դ�Ĵ���ɫshader
	whiteShader = std::make_shared<Shader>("Shader/Sample4 Light/whiteVertex.glsl", "Shader/Sample4 Light/whiteFrag.glsl");
	// ��ʼ����Ⱦ�����phong����ģ����ɫ��
	lightObjectShader = std::make_shared<Shader>("Shader/Sample4 Light/phongVertex.glsl", "Shader/Sample4 Light/phongFrag.glsl");

	// ��ʼ��VAO����,Ȼ���
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	// ����VBO����,���������������뵽�˻�����
	float vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	0.0f,  0.0f, -1.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 0.0f,	0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  0.0f, -1.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  0.0f, -1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  0.0f, -1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,	0.0f,  0.0f, -1.0f, 

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,	 0.0f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 -1.0f,  0.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 -1.0f,  0.0f,  0.0f,

	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 1.0f,  0.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 1.0f,  0.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	 1.0f,  0.0f,  0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f, -0.5f,  1.0f, 1.0f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
	 0.5f, -0.5f,  0.5f,  1.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,	 0.0f, -1.0f,  0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,	 0.0f, -1.0f,  0.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f, -0.5f,  1.0f, 1.0f,	0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	0.0f,  1.0f,  0.0f,
	 0.5f,  0.5f,  0.5f,  1.0f, 0.0f,	0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,	0.0f,  1.0f,  0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,	0.0f,  1.0f,  0.0f,
	};
	glGenBuffers(1,&VBO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(vertices),vertices,GL_STATIC_DRAW);

	// ���ò����ö�������

	// pos
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,8*sizeof(float),0);
	glEnableVertexAttribArray(0);

	// uv
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,8*sizeof(float),(void *)(3*sizeof(float)));
	glEnableVertexAttribArray(1);

	// normal
	glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,8*sizeof(float),(void*)(5*sizeof(float)));
	glEnableVertexAttribArray(2);
}

void Sample4::render() {
	// ��VAO
	glBindVertexArray(VAO);

	// ��Դ��ģ�;���
	glm::mat4 lightModel(1.0f);
	lightModel = glm::translate(lightModel,lightPos);
	lightModel = glm::scale(lightModel,glm::vec3(0.2f));

	// ��ù۲�����ͶӰ����
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projMatrix = camera->getProjectionMatrix();
	whiteShader->use();
	whiteShader->setMatrix4x4("model",glm::value_ptr(lightModel));
	whiteShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	whiteShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);

	// ���ƹ�Դ
	glDrawArrays(GL_TRIANGLES, 0, 36);


	// ��������
	lightObjectShader->use();
	// ��������
	lightObjectShader->setTexture2D("MainTex",*image,0);
	// ���ù�Դλ��
	lightObjectShader->setFloat3("lightPos",lightPos.x,lightPos.y,lightPos.z);
	// ���ù�Դ��ɫ
	lightObjectShader->setFloat4("lightColor",lightColor.x,lightColor.y,lightColor.z,lightColor.w);
	// ���ù۲�λ��
	lightObjectShader->setFloat3("viewPos",camera->pos.x, camera->pos.y, camera->pos.z);

	glm::mat4 objectModel(1.0);
	lightObjectShader->setMatrix4x4("model", glm::value_ptr(objectModel));
	lightObjectShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	lightObjectShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));
	// ��������
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Sample4::onDestory() {
	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1,&VAO);
}
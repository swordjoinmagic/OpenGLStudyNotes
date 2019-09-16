#include "Sample12_Instaces_Planet.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// ��ʯ����
unsigned int amount = 10000;

// ģ�;�������
glm::mat4 *modelMatrices = new glm::mat4[amount];

void Sample12Planet::init() {
	// ��ʼ��ģ��
	rock = std::make_shared<Model>("mesh/rock.obj");
	planet = std::make_shared<Model>("mesh/planet.obj");

	// ��ʼ��Shader
	rockShader = std::make_shared<Shader>("Shader/Sample12 Instances/rockV.glsl", "Shader/Sample12 Instances/planetF.glsl");
	planetShader = std::make_shared<Shader>("Shader/Sample12 Instances/planetV.glsl", "Shader/Sample12 Instances/planetF.glsl");

	// ��ʼ������	
	rockTexture = std::make_shared<Image>("Image/Rock-Texture-Surface.jpg");
	planetTexture = std::make_shared<Image>("Image/wall.jpg");

	// ��ʼ���������
	srand(glfwGetTime());
	float radius = 50;
	float offset = 2.5f;
	// ��ʼ��ģ�;�������
	for (unsigned int i = 0; i < amount;i++) {
		glm::mat4 model(1.0f);

		// //3. λ��: �ֲ��ڰ뾶Ϊ"radiius"��Բ����,ƫ�Ʒ�ΧΪ[-offset,offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 1. ����:��0.05��0.25֮������
		float scale = (rand() % 20) / 100.0f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));

		 //2. ��ת: ��һ�����ѡ�����ת�����������������ת
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// ��ӵ������������
		modelMatrices[i] = model;
	}
	//
	//// ����ʵ������
	unsigned int buffer;
	glGenBuffers(1,&buffer);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glBufferData(GL_ARRAY_BUFFER,amount * sizeof(glm::mat4),&modelMatrices[0],GL_STATIC_DRAW);

	// ��ÿ��rock���ò�ͬ��model����
	for (unsigned int i = 0; i < rock->meshes.size(); i++) {
		unsigned int VAO = rock->meshes[i].getVAO();
		// �󶨶����������
		glBindVertexArray(VAO);

		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3,4,GL_FLOAT,GL_FALSE,sizeof(glm::mat4),(void*)0);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4,4,GL_FLOAT,GL_FALSE, sizeof(glm::mat4),(void*)(sizeof(glm::vec4)));
		glEnableVertexAttribArray(5);
		glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 2));
		glEnableVertexAttribArray(6);
		glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4) * 3));

		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);
		glVertexAttribDivisor(5, 1);
		glVertexAttribDivisor(6, 1);

		glBindVertexArray(0);
	}
}

void Sample12Planet::render() {

	glEnable(GL_DEPTH_TEST);

	rockShader->use();
	// ����mvp����
	//rockShader->setMatrix4x4("model",glm::value_ptr(glm::mat4(1.0f)));
	rockShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	rockShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	rockShader->setTexture2D("MainTex", *rockTexture, 0);
	for (unsigned int i = 0; i < rock->meshes.size(); i++) {
		glBindVertexArray(rock->meshes[i].getVAO());
		glDrawElementsInstanced(GL_TRIANGLES, rock->meshes[i].indices.size(),GL_UNSIGNED_INT,0,amount);

	}
}

void Sample12Planet::onDestory() {}
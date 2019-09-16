#include "Sample12_Instaces_Planet.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
// 陨石数量
unsigned int amount = 10000;

// 模型矩阵数组
glm::mat4 *modelMatrices = new glm::mat4[amount];

void Sample12Planet::init() {
	// 初始化模型
	rock = std::make_shared<Model>("mesh/rock.obj");
	planet = std::make_shared<Model>("mesh/planet.obj");

	// 初始化Shader
	rockShader = std::make_shared<Shader>("Shader/Sample12 Instances/rockV.glsl", "Shader/Sample12 Instances/planetF.glsl");
	planetShader = std::make_shared<Shader>("Shader/Sample12 Instances/planetV.glsl", "Shader/Sample12 Instances/planetF.glsl");

	// 初始化纹理	
	rockTexture = std::make_shared<Image>("Image/Rock-Texture-Surface.jpg");
	planetTexture = std::make_shared<Image>("Image/wall.jpg");

	// 初始化随机种子
	srand(glfwGetTime());
	float radius = 50;
	float offset = 2.5f;
	// 初始化模型矩阵数组
	for (unsigned int i = 0; i < amount;i++) {
		glm::mat4 model(1.0f);

		// //3. 位移: 分布在半径为"radiius"的圆形上,偏移范围为[-offset,offset]
		float angle = (float)i / (float)amount * 360.0f;
		float displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float x = sin(angle) * radius + displacement;
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float y = displacement * 0.4f; // keep height of asteroid field smaller compared to width of x and z
		displacement = (rand() % (int)(2 * offset * 100)) / 100.0f - offset;
		float z = cos(angle) * radius + displacement;
		model = glm::translate(model, glm::vec3(x, y, z));

		// 1. 缩放:在0.05和0.25之间缩放
		float scale = (rand() % 20) / 100.0f + 0.05f;
		model = glm::scale(model, glm::vec3(scale));

		 //2. 旋转: 绕一个随机选择的旋转轴向量进行随机的旋转
		float rotAngle = (rand() % 360);
		model = glm::rotate(model, rotAngle, glm::vec3(0.4f, 0.6f, 0.8f));

		// 添加到矩阵的数组中
		modelMatrices[i] = model;
	}
	//
	//// 创建实例数组
	unsigned int buffer;
	glGenBuffers(1,&buffer);
	glBindBuffer(GL_ARRAY_BUFFER,buffer);
	glBufferData(GL_ARRAY_BUFFER,amount * sizeof(glm::mat4),&modelMatrices[0],GL_STATIC_DRAW);

	// 对每个rock设置不同的model矩阵
	for (unsigned int i = 0; i < rock->meshes.size(); i++) {
		unsigned int VAO = rock->meshes[i].getVAO();
		// 绑定顶点数组对象
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
	// 设置mvp矩阵
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
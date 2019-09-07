#include "Sample4_Light.h"

// ��Դλ��
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
// ��Դ��ɫ
glm::vec4 lightColor(1,1,1,1);
// ƽ�йⷽ��
glm::vec3 lightDirection(1.2f,1.0f,2.0f);

// ʮ������λ��
glm::vec3 cubePositions[] = {
	glm::vec3(0.0f,  0.0f,  0.0f),
	glm::vec3(2.0f,  5.0f, -15.0f),
	glm::vec3(-1.5f, -2.2f, -2.5f),
	glm::vec3(-3.8f, -2.0f, -12.3f),
	glm::vec3(2.4f, -0.4f, -3.5f),
	glm::vec3(-1.7f,  3.0f, -7.5f),
	glm::vec3(1.3f, -2.0f, -2.5f),
	glm::vec3(1.5f,  2.0f, -2.5f),
	glm::vec3(1.5f,  0.2f, -1.5f),
	glm::vec3(-1.3f,  1.0f, -1.5f)
};

void Sample4::init() {
	// ��ʼ���������
	image = std::make_shared<SJM::Image>("Image/container.jpg");

	// ��ʼ����Ⱦ��Դ�Ĵ���ɫshader
	whiteShader = std::make_shared<Shader>("Shader/Sample4 Light/whiteVertex.glsl", "Shader/Sample4 Light/whiteFrag.glsl");
	// ��ʼ����Ⱦ�����phong����ģ����ɫ��
	lightObjectShader = std::make_shared<Shader>("Shader/Sample4 Light/phongVertex.glsl", "Shader/Sample4 Light/phongFrag.glsl");
	// ��ʼ��ƽ�й�Shader
	directionalShader = std::make_shared<Shader>("Shader/Sample4 Light/DirectionalVertex.glsl", "Shader/Sample4 Light/DirectionFrag.glsl");
	// ��ʼ�����ԴShader
	pointShader = std::make_shared<Shader>("Shader/Sample4 Light/PointlVertex.glsl", "Shader/Sample4 Light/PointFrag.glsl");
	// ��ʼ���۹�ƹ�Դ��Shader
	spotShader = std::make_shared<Shader>("Shader/Sample4 Light/SpotVertex.glsl", "Shader/Sample4 Light/SpotFrag.glsl");

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
	glm::mat4 objectModel(1.0);

	//lightObjectShader->use();
	//// ��������
	//lightObjectShader->setTexture2D("MainTex",*image,0);
	//// ���ù�Դλ��
	//lightObjectShader->setFloat3("lightPos",lightPos.x,lightPos.y,lightPos.z);
	//// ���ù�Դ��ɫ
	//lightObjectShader->setFloat4("lightColor",lightColor.x,lightColor.y,lightColor.z,lightColor.w);
	//// ���ù۲�λ��
	//lightObjectShader->setFloat3("viewPos",camera->pos.x, camera->pos.y, camera->pos.z);
	//lightObjectShader->setMatrix4x4("model", glm::value_ptr(objectModel));
	//lightObjectShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	//lightObjectShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));

	// ʹ��ƽ�й�Shader��������
	//lightDirection = glm::normalize(lightDirection);
	//directionalShader->use();
	//directionalShader->setTexture2D("MainTex", *image, 0);
	//directionalShader->setFloat3("viewPos", camera->pos.x, camera->pos.y, camera->pos.z);
	//directionalShader->setFloat3("light.direction",lightDirection.x, lightDirection.y, lightDirection.z);
	//directionalShader->setFloat3("light.ambient",0.2f,0.2f,0.2f);
	//directionalShader->setFloat3("light.diffuse",1.0f,1.0f,1.0f);
	//directionalShader->setFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	//// ����mvp����	
	//directionalShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	//directionalShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));	
	
	// ʹ�õ��ԴShader��������
	//pointShader->use();
	//// ����mvp����	
	//pointShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	//pointShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));
	//// ���ò���
	//pointShader->setFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	//pointShader->setFloat3("light.diffuse", 1.0f, 1.0f, 1.0f);
	//pointShader->setFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	//pointShader->setFloat3("light.lightPos", lightPos.x, lightPos.y, lightPos.z);
	//pointShader->setFloat3("viewPos", camera->pos.x, camera->pos.y, camera->pos.z);
	//pointShader->setTexture2D("MainTex", *image, 0);
	//// ����˥������
	//pointShader->setFloat("light.constant",1.0f);
	//pointShader->setFloat("light.linear",0.09f);
	//pointShader->setFloat("light.quadratic",0.032f);

	// ʹ�þ۹����������
	spotShader->use();
	// ����mvp����	
	spotShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	spotShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));
	// ���ò���
	spotShader->setFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	spotShader->setFloat3("light.diffuse", 1.0f, 1.0f, 1.0f);
	spotShader->setFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	spotShader->setFloat3("light.lightPos", camera->pos.x, camera->pos.y, camera->pos.z);
	spotShader->setFloat3("viewPos", camera->pos.x, camera->pos.y, camera->pos.z);
	spotShader->setFloat3("light.spotDirection",camera->front.x, camera->front.y, camera->front.z);
	spotShader->setFloat("light.cutOff",glm::cos(glm::radians(12.5f)));
	spotShader->setFloat("light.innerCone",glm::cos(glm::radians(7.5f)));
	spotShader->setTexture2D("MainTex", *image, 0);
	// ����˥������
	spotShader->setFloat("light.constant",1.0f);
	spotShader->setFloat("light.linear",0.09f);
	spotShader->setFloat("light.quadratic",0.032f);

	// ����N������
	for (int i = 0; i < 10; i++) {
		glm::mat4 model(1.0f);
		model = glm::translate(model,cubePositions[i]);
		float angle = 20.0f * i;
		model = glm::rotate(model,glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));

		directionalShader->setMatrix4x4("model", glm::value_ptr(model));

		glDrawArrays(GL_TRIANGLES, 0, 36);
	}
}

void Sample4::onDestory() {
	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1,&VAO);
}
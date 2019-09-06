#include "Sample4_Light.h"

// 光源位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
// 光源颜色
glm::vec4 lightColor(1,1,1,1);

void Sample4::init() {

	// 初始化纹理对象
	image = std::make_shared<SJM::Image>("Image/container.jpg");

	// 初始化渲染光源的纯白色shader
	whiteShader = std::make_shared<Shader>("Shader/Sample4 Light/whiteVertex.glsl", "Shader/Sample4 Light/whiteFrag.glsl");
	// 初始化渲染物体的phong光照模型着色器
	lightObjectShader = std::make_shared<Shader>("Shader/Sample4 Light/phongVertex.glsl", "Shader/Sample4 Light/phongFrag.glsl");

	// 初始化VAO对象,然后绑定
	glGenVertexArrays(1,&VAO);
	glBindVertexArray(VAO);

	// 生成VBO对象,并将顶点数据输入到此缓冲中
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

	// 设置并启用顶点属性

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
	// 绑定VAO
	glBindVertexArray(VAO);

	// 光源的模型矩阵
	glm::mat4 lightModel(1.0f);
	lightModel = glm::translate(lightModel,lightPos);
	lightModel = glm::scale(lightModel,glm::vec3(0.2f));

	// 获得观察矩阵和投影矩阵
	glm::mat4 viewMatrix = camera->getViewMatrix();
	glm::mat4 projMatrix = camera->getProjectionMatrix();
	whiteShader->use();
	whiteShader->setMatrix4x4("model",glm::value_ptr(lightModel));
	whiteShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	whiteShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);

	// 绘制光源
	glDrawArrays(GL_TRIANGLES, 0, 36);


	// 绘制物体
	lightObjectShader->use();
	// 设置纹理
	lightObjectShader->setTexture2D("MainTex",*image,0);
	// 设置光源位置
	lightObjectShader->setFloat3("lightPos",lightPos.x,lightPos.y,lightPos.z);
	// 设置光源颜色
	lightObjectShader->setFloat4("lightColor",lightColor.x,lightColor.y,lightColor.z,lightColor.w);
	// 设置观察位置
	lightObjectShader->setFloat3("viewPos",camera->pos.x, camera->pos.y, camera->pos.z);

	glm::mat4 objectModel(1.0);
	lightObjectShader->setMatrix4x4("model", glm::value_ptr(objectModel));
	lightObjectShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	lightObjectShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));
	// 绘制物体
	glDrawArrays(GL_TRIANGLES, 0, 36);
}

void Sample4::onDestory() {
	glDeleteBuffers(1,&VBO);
	glDeleteVertexArrays(1,&VAO);
}
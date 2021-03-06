#include "Sample4_Light.h"

// 光源位置
glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
// 光源颜色
glm::vec4 lightColor(1,1,1,1);
// 平行光方向
glm::vec3 lightDirection(1.2f,1.0f,2.0f);

// 十个箱子位置
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
	// 初始化纹理对象
	image = std::make_shared<SJM::Image>("Image/container.jpg");

	// 初始化渲染光源的纯白色shader
	whiteShader = std::make_shared<Shader>("Shader/Sample4 Light/whiteVertex.glsl", "Shader/Sample4 Light/whiteFrag.glsl");
	// 初始化渲染物体的phong光照模型着色器
	lightObjectShader = std::make_shared<Shader>("Shader/Sample4 Light/phongVertex.glsl", "Shader/Sample4 Light/phongFrag.glsl");
	// 初始化平行光Shader
	directionalShader = std::make_shared<Shader>("Shader/Sample4 Light/DirectionalVertex.glsl", "Shader/Sample4 Light/DirectionFrag.glsl");
	// 初始化点光源Shader
	pointShader = std::make_shared<Shader>("Shader/Sample4 Light/PointlVertex.glsl", "Shader/Sample4 Light/PointFrag.glsl");
	// 初始化聚光灯光源的Shader
	spotShader = std::make_shared<Shader>("Shader/Sample4 Light/SpotVertex.glsl", "Shader/Sample4 Light/SpotFrag.glsl");

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
	glm::mat4 objectModel(1.0);

	//lightObjectShader->use();
	//// 设置纹理
	//lightObjectShader->setTexture2D("MainTex",*image,0);
	//// 设置光源位置
	//lightObjectShader->setFloat3("lightPos",lightPos.x,lightPos.y,lightPos.z);
	//// 设置光源颜色
	//lightObjectShader->setFloat4("lightColor",lightColor.x,lightColor.y,lightColor.z,lightColor.w);
	//// 设置观察位置
	//lightObjectShader->setFloat3("viewPos",camera->pos.x, camera->pos.y, camera->pos.z);
	//lightObjectShader->setMatrix4x4("model", glm::value_ptr(objectModel));
	//lightObjectShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	//lightObjectShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));

	// 使用平行光Shader照亮物体
	//lightDirection = glm::normalize(lightDirection);
	//directionalShader->use();
	//directionalShader->setTexture2D("MainTex", *image, 0);
	//directionalShader->setFloat3("viewPos", camera->pos.x, camera->pos.y, camera->pos.z);
	//directionalShader->setFloat3("light.direction",lightDirection.x, lightDirection.y, lightDirection.z);
	//directionalShader->setFloat3("light.ambient",0.2f,0.2f,0.2f);
	//directionalShader->setFloat3("light.diffuse",1.0f,1.0f,1.0f);
	//directionalShader->setFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	//// 设置mvp矩阵	
	//directionalShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	//directionalShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));	
	
	// 使用点光源Shader照亮物体
	//pointShader->use();
	//// 设置mvp矩阵	
	//pointShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	//pointShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));
	//// 设置材质
	//pointShader->setFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	//pointShader->setFloat3("light.diffuse", 1.0f, 1.0f, 1.0f);
	//pointShader->setFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	//pointShader->setFloat3("light.lightPos", lightPos.x, lightPos.y, lightPos.z);
	//pointShader->setFloat3("viewPos", camera->pos.x, camera->pos.y, camera->pos.z);
	//pointShader->setTexture2D("MainTex", *image, 0);
	//// 设置衰减因子
	//pointShader->setFloat("light.constant",1.0f);
	//pointShader->setFloat("light.linear",0.09f);
	//pointShader->setFloat("light.quadratic",0.032f);

	// 使用聚光灯照亮物体
	spotShader->use();
	// 设置mvp矩阵	
	spotShader->setMatrix4x4("view", glm::value_ptr(viewMatrix));
	spotShader->setMatrix4x4("projection", glm::value_ptr(projMatrix));
	// 设置材质
	spotShader->setFloat3("light.ambient", 0.2f, 0.2f, 0.2f);
	spotShader->setFloat3("light.diffuse", 1.0f, 1.0f, 1.0f);
	spotShader->setFloat3("light.specular", 1.0f, 1.0f, 1.0f);
	spotShader->setFloat3("light.lightPos", camera->pos.x, camera->pos.y, camera->pos.z);
	spotShader->setFloat3("viewPos", camera->pos.x, camera->pos.y, camera->pos.z);
	spotShader->setFloat3("light.spotDirection",camera->front.x, camera->front.y, camera->front.z);
	spotShader->setFloat("light.cutOff",glm::cos(glm::radians(12.5f)));
	spotShader->setFloat("light.innerCone",glm::cos(glm::radians(7.5f)));
	spotShader->setTexture2D("MainTex", *image, 0);
	// 设置衰减因子
	spotShader->setFloat("light.constant",1.0f);
	spotShader->setFloat("light.linear",0.09f);
	spotShader->setFloat("light.quadratic",0.032f);

	// 绘制N个物体
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
#include "Sample13_Shadow.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;
const unsigned int SCREEN_WIDTH = 800, SCREEN_HEIGHT=600;


// 屏幕四边形的顶点属性
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

#pragma region 初始化点光源深度图帧缓冲(CubeMap)

	// 初始化光源位置
	lightPos = std::make_shared<glm::vec3>(0,1.5f,0);

	// 初始化立方体贴图
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
	// 设置cubeMap的采样和过滤模式
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_CUBE_MAP,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

	// 初始化帧缓冲
	glGenFramebuffers(1,&cubeMapDepthMapFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,cubeMapDepthMapFBO);

	// 添加深度缓冲附件
	glFramebufferTexture(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,depthCubeMap,0);
	// 不使用颜色缓冲进行渲染
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);

	// 解绑帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,0);

#pragma endregion

#pragma region 平行光深度图帧缓冲

	// 为渲染的深度贴图创建一个帧缓冲对象
	glGenFramebuffers(1,&depthMapFBO);

	// 创建一个2D纹理,供帧缓冲的深度缓冲使用
	glGenTextures(1,&depthMap);
	glBindTexture(GL_TEXTURE_2D,depthMap);
	glTexImage2D(GL_TEXTURE_2D,0,GL_DEPTH_COMPONENT,SHADOW_WIDTH,SHADOW_HEIGHT,0,GL_DEPTH_COMPONENT,GL_FLOAT,NULL);
	// 设置采样以及过滤模式
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
	GLfloat borderColor[] = {1.0,1.0,1.0,1.0};
	glTexParameterfv(GL_TEXTURE_2D,GL_TEXTURE_BORDER_COLOR,borderColor);

	// 把生成的深度纹理作为帧缓冲的深度缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_TEXTURE_2D,depthMap,0);
	// 显式告诉GL我们不使用任何颜色数据进行渲染
	glDrawBuffer(GL_NONE);
	glReadBuffer(GL_NONE);
	// 解绑帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,0);

#pragma endregion

#pragma region 后处理帧缓冲

	// 生成后处理帧缓冲对象
	glGenFramebuffers(1,&postEffectFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,postEffectFBO);

	// 生成并绑定纹理(用于颜色缓冲)
	glGenTextures(1,&texColorBuffer);
	glBindTexture(GL_TEXTURE_2D,texColorBuffer);
	// 生成一个空的纹理,与屏幕宽高一样
	glTexImage2D(GL_TEXTURE_2D,0,GL_RGB,SCREEN_WIDTH,SCREEN_HEIGHT,0,GL_RGB,GL_UNSIGNED_BYTE,NULL);
	// 设置纹理过滤模式
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
	// 解绑纹理
	glBindTexture(GL_TEXTURE_2D,0);

	// 将新生成的纹理对象以附件(attachment)的形式添加到当前帧缓冲对象中
	glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,texColorBuffer,0);


	// 创建渲染缓冲对象(只可读不可写),用于深度/模板缓冲
	glGenRenderbuffers(1,&rbo);
	glBindRenderbuffer(GL_RENDERBUFFER,rbo);
	// 设置格式
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH24_STENCIL8,SCREEN_WIDTH,SCREEN_HEIGHT);

	// 将渲染缓冲对象附加到当前帧缓冲对象
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_STENCIL_ATTACHMENT,GL_RENDERBUFFER,rbo);

	// 检查帧缓冲是否完整,如果不完整,打印错误信息
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "帧缓冲不完整!!!" << std::endl;
	}

	// 解绑帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,0);
#pragma endregion

	// 初始化屏幕四边形的顶点属性
	unsigned int quadVBO;
	glGenVertexArrays(1,&quadVAO);
	glBindVertexArray(quadVAO);
	glGenBuffers(1,&quadVBO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),quadVertices,GL_STATIC_DRAW);

	// 设置并启用顶点属性

	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);
	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

	// 解绑VBO,VAO
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

	// 初始化Model
	cubeModel = std::make_shared<Cube>();
	// 初始化Shader
	depthShader = std::make_shared<Shader>("Shader/Sample13 Shadow/depthV.glsl", "Shader/Sample13 Shadow/depthF.glsl");
	objShader = std::make_shared<Shader>("Shader/Sample13 Shadow/v.glsl", "Shader/Sample13 Shadow/f.glsl");
	objTexture = std::make_shared<Image>("Image/container.jpg");
	floorTexture = std::make_shared<Image>("Image/wall.jpg");
	postEffectShader = std::make_shared<Shader>("Shader/Sample13 Shadow/postEffectV.glsl", "Shader/Sample13 Shadow/postEffectF.glsl");
	
	cubeMapDepthShader = std::make_shared<Shader>("Shader/Sample13 Shadow/pointShadowCreateCubeMapV.glsl", "Shader/Sample13 Shadow/pointShadowCreateCubeMapF.glsl", "Shader/Sample13 Shadow/pointShadowCreateCubeMapG.glsl");
}

void Sample13::render() {

#pragma region 渲染点光源深度图

	// 绑定帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,cubeMapDepthMapFBO);
	// 设置视口
	glViewport(0,0,SHADOW_WIDTH,SHADOW_HEIGHT);
	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	// 清空该帧缓冲的深度缓冲
	glClear(GL_DEPTH_BUFFER_BIT);

	// 设置光源空间的投影矩阵
	GLfloat aspect = (GLfloat)SHADOW_WIDTH / (GLfloat)SHADOW_HEIGHT;
	GLfloat near1 = 1.0f;
	GLfloat far1 = 25.0f;
	glm::mat4 pointShaderProj = glm::perspective(glm::radians(90.0f),aspect,near1,far1);

	// 设置观察矩阵(6个面的)
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
	// 设置光源空间vp矩阵
	for (unsigned int i = 0; i < 6; i++) {
		cubeMapDepthShader->setMatrix4x4("shadowMatrices["+std::to_string(i)+"]",glm::value_ptr(pointShadowTransforms[i]));
	}
	renderScene(cubeMapDepthShader.get());

	// 恢复默认视口
	glViewport(0,0,SCREEN_WIDTH,SCREEN_HEIGHT);
	// 解绑帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,0);

#pragma endregion

#pragma region 渲染平行光深度图

	// 剔除正面
	glCullFace(GL_FRONT);

	// 先绑定渲染深度图的帧缓冲渲染一遍,以得到光源空间的深度图
	glViewport(0,0,SHADOW_WIDTH,SHADOW_HEIGHT);
	glBindFramebuffer(GL_FRAMEBUFFER,depthMapFBO);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	// 创建光源空间的视图和投影矩阵
	float near_plane = 1.0f,far_plane = 7.5f;
	glm::mat4 lightProjection = glm::ortho(-10.0f,10.0f,-10.0f,10.0f,near_plane,far_plane);
	glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f,4.0f,-1.0f),glm::vec3(0.0f),glm::vec3(1.0f));

	depthShader->use();
	depthShader->setMatrix4x4("view", glm::value_ptr(lightView));
	depthShader->setMatrix4x4("projection", glm::value_ptr(lightProjection));

	renderScene(depthShader.get());

	// 恢复剔除
	glCullFace(GL_BACK);
#pragma endregion

#pragma region 渲染后处理帧缓冲(即将正常场景渲染到一张RenderTexture上)

	glViewport(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT);
	// 绑定后处理帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,postEffectFBO);
	glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	objShader->use();
	objShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	objShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	objShader->setMatrix4x4("lightView",glm::value_ptr(lightView));
	objShader->setMatrix4x4("lightProjection",glm::value_ptr(lightProjection));	
	// 设置平行光深度贴图
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D,depthMap);
	objShader->setInt("depthMap",1);

	// 设置点光源深度贴图
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_CUBE_MAP,depthCubeMap);
	objShader->setInt("pointDepthMap",2);
	// 设置点光源位置
	objShader->setFloat3("lightPos",lightPos->x, lightPos->y, lightPos->z);
	// 设置光源空间远平面
	objShader->setFloat("far_plane",far1);

	renderScene(objShader.get());


#pragma endregion

#pragma region 将后处理帧缓冲渲染到屏幕四边形上

	// 绑定默认帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	// 关闭深度测试
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
	// 绘制地板

	shader->use();

	// 地板的Model
	glm::mat4 floormMatrix(1.0f);
	floormMatrix = glm::translate(floormMatrix, glm::vec3(0, -0.5f, 0));
	floormMatrix = glm::scale(floormMatrix, glm::vec3(25, 0.1f, 25));
	// 设置mvp矩阵
	shader->setMatrix4x4("model", glm::value_ptr(floormMatrix));
	// 设置纹理
	shader->setTexture2D("MainTex", *floorTexture, 0);

	// 绘制物体
	cubeModel->Draw(*shader);

	//========================
	// 绘制立方体

	shader->use();
	shader->setTexture2D("MainTex", *objTexture, 0);

	// 立方体的Model
	glm::mat4 mMatrix(1.0f);
	// 绘制两个立方体
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

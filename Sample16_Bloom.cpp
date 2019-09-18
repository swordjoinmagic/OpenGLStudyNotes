#include "Sample16_Bloom.h"

const float screenQuadVertices[] = { // vertex attributes for a quad that fills the entire screen in Normalized Device Coordinates.
	// positions		 // texCoords
	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	-1.0f, -1.0f, 0.0f,  0.0f, 0.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,

	-1.0f,  1.0f, 0.0f,  0.0f, 1.0f,
	 1.0f, -1.0f, 0.0f,  1.0f, 0.0f,
	 1.0f,  1.0f, 0.0f,  1.0f, 1.0f
};

void Sample16::init() {

#pragma region 初始化后处理帧缓冲

	glGenFramebuffers(1,&postEffectFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,postEffectFBO);

	glGenTextures(2, postEffectColorBuffer);

	// 初始化后处理帧缓冲中的颜色缓冲,
	// 这里初始化两个,一个用于渲染场景,一个用于提取亮色
	for (unsigned int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, postEffectColorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		// 设置纹理过滤模式为线性采样
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// 将当前纹理对象附加到当前的帧缓冲对象
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, postEffectColorBuffer[i], 0);

	}
	// 检查帧缓冲是否完整,如果不完整,打印错误信息
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "帧缓冲不完整!!!" << std::endl;
	}

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// 解绑帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region 初始化高斯模糊帧缓冲

	glGenFramebuffers(2,blurFBO);
	glGenTextures(2,blurColorBuffer);
	for (unsigned int i = 0; i < 2;i++) {
		glBindFramebuffer(GL_FRAMEBUFFER,blurFBO[i]);
		glBindTexture(GL_TEXTURE_2D,blurColorBuffer[i]);

		// 创建纹理并设置采样/过滤模式
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,800,600,0,GL_RGB,GL_FLOAT,NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// 将该纹理添加进当前帧缓冲中
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,blurColorBuffer[i],0);

	}

#pragma endregion


#pragma region 初始化屏幕四边形

	unsigned int screenQuadVBO;
	// 初始化屏幕四边形
	glGenVertexArrays(1,&screenQuadVAO);
	glBindVertexArray(screenQuadVAO);
	glGenBuffers(1,&screenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER,screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(screenQuadVertices),screenQuadVertices,GL_STATIC_DRAW);

	// 设置并启用顶点属性
	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);
	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));

	// 解绑vao,vbo
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

#pragma endregion

	// 初始化后处理Shader和Image
	texture = std::make_shared<Image>("Image/sakura1.jpg");
	bloomShader = std::make_shared<Shader>("Shader/Sample16 Bloom/bloomV.glsl", "Shader/Sample16 Bloom/bloomF.glsl");
	postEffectShader = std::make_shared<Shader>("Shader/Sample16 Bloom/postEffectV.glsl", "Shader/Sample16 Bloom/postEffectF.glsl");
	blurShader = std::make_shared<Shader>("Shader/Sample16 Bloom/postEffectV.glsl", "Shader/Sample16 Bloom/blurF.glsl");
}

void Sample16::render() {

#pragma region 渲染场景并提取亮色
	glBindFramebuffer(GL_FRAMEBUFFER, postEffectFBO);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// 清除颜色缓冲和深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	bloomShader->use();

	bloomShader->setTexture2D("MainTex", *texture, 0);

	// 渲染场景
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion

#pragma region 对亮色两步高斯模糊

	GLboolean horizontal = true,first_iteration = true;
	GLuint amount = 50;
	blurShader->use();
	// 每两次是一次高斯模糊,即对一张图进行迭代5次的高斯模糊操作
	for (unsigned int i = 0; i < amount; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER,blurFBO[horizontal]);
		blurShader->setBool("horizontal",horizontal);
		// 设置当前要进行模糊的图像,
		// 如果是第一次迭代,那么当前要模糊的图像即为渲染场景时得到的亮色图
		// 如果不是第一次迭代,那么当前要渲染的图像即为上一次迭代时得到的图像
		glBindTexture(GL_TEXTURE_2D,first_iteration?postEffectColorBuffer[1]:blurColorBuffer[!horizontal]);
		blurShader->setInt("image",0);

		glBindVertexArray(screenQuadVAO);
		glDrawArrays(GL_TRIANGLES,0,6);

		horizontal = !horizontal;
		if (first_iteration) first_iteration = false;
	}

	// 解绑
	glBindFramebuffer(GL_FRAMEBUFFER,0);
#pragma endregion


	// 绑定默认帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	glDisable(GL_DEPTH_TEST);

	glBindVertexArray(screenQuadVAO);
	postEffectShader->use();
	glBindTexture(GL_TEXTURE_2D,postEffectColorBuffer[0]);
	postEffectShader->setInt("ScreenTex",0);
	glActiveTexture(GL_TEXTURE0 + 1);
	glBindTexture(GL_TEXTURE_2D,blurColorBuffer[0]);
	postEffectShader->setInt("blurTex",1);

	// 渲染后处理场景
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// 解绑
	glBindVertexArray(0);
}

void Sample16::onDestory() {}

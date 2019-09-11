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
	// 初始化场景四边形对象
	glGenVertexArrays(1,&quadVAO);
	glGenBuffers(1,&quadVBO);
	glBindVertexArray(quadVAO);
	glBindBuffer(GL_ARRAY_BUFFER,quadVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(quadVertices),&quadVertices,GL_STATIC_DRAW);

	// 启用顶点属性

	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)0);

	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,5*sizeof(float),(void*)(3*sizeof(float)));

	// 恢复默认绑定对象
	glBindVertexArray(0);


	// 初始化模型对象
	obj = std::make_shared<Model>("mesh/quad.obj");

	// 初始化shader和纹理
	objShader = std::make_shared<Shader>("Shader/Sample9 PostEffect/objVertex.glsl", "Shader/Sample9 PostEffect/objFrag.glsl");
	objTexture = std::make_shared<Image>("Image/wall.jpg");

	quadShader = std::make_shared<Shader>("Shader/Sample9 PostEffect/postEffectVertex.glsl", "Shader/Sample9 PostEffect/postEffectFrag.glsl");

	// 生成一个帧缓冲对象并绑定它
	glGenFramebuffers(1, &framebuffer);
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

	// 生成纹理(用于颜色缓冲)
	glGenTextures(1, &texColorBuffer);
	glBindTexture(GL_TEXTURE_2D, texColorBuffer);
	// 生成一个空的纹理,大小与屏幕宽高一样
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
	// 设置纹理过滤模式为线性采样
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	// 恢复默认的绑定纹理
	glBindTexture(GL_TEXTURE_2D, 0);

	// 将当前纹理对象附加到当前的帧缓冲对象
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColorBuffer, 0);

	// 创建渲染缓冲对象
	glGenRenderbuffers(1, &rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, 800, 600);
	
	// 将渲染缓冲对象绑定至当前帧缓冲对象
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER,rbo);

	// 检查帧缓冲是否完整,如果不完整,打印错误信息
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER)!=GL_FRAMEBUFFER_COMPLETE){
		std::cout << "帧缓冲不完整!!!" << std::endl;
	}

	// 恢复默认状态
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Sample9::render() {
	// 激活新建的帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,framebuffer);
	glClearColor(0.1f,0.1f,0.1f,1.0f);
	// 清除颜色缓冲和深度缓冲
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	objShader->use();
	objShader->setMatrix4x4("view",glm::value_ptr(camera->getViewMatrix()));
	objShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));

	objShader->setTexture2D("MainTex",*objTexture,0);

	// 绘制场景到帧缓冲中去
	for (int i = 0; i < objectPosotion->length(); i++) {
		glm::mat4 model(1.0f);
		model = glm::translate(model,objectPosotion[i]);
		objShader->setMatrix4x4("model",glm::value_ptr(model));

		obj->Draw(*objShader);
	}

	// 第二个绘制阶段,将新建帧缓冲中的颜色缓冲(纹理)渲染
	// 到场景中的一个四边形上(quad)
	
	// 首先绑定默认帧缓冲
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glClearColor(1.0f,1.0f,1.0f,1.0f);
	glClear(GL_COLOR_BUFFER_BIT);

	// 使用后处理Shader处理整个屏幕花画面
	quadShader->use();
	// 绑定屏幕四边形的顶点数组对象
	glBindVertexArray(quadVAO);
	// 关闭深度测试
	glDisable(GL_DEPTH_TEST);
	glBindTexture(GL_TEXTURE_2D,texColorBuffer);
	quadShader->setInt("screenTex",0);

	glDrawArrays(GL_TRIANGLES,0,6);

}

void Sample9::onDestory() {}
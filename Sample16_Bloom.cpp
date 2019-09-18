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

#pragma region ��ʼ������֡����

	glGenFramebuffers(1,&postEffectFBO);
	glBindFramebuffer(GL_FRAMEBUFFER,postEffectFBO);

	glGenTextures(2, postEffectColorBuffer);

	// ��ʼ������֡�����е���ɫ����,
	// �����ʼ������,һ��������Ⱦ����,һ��������ȡ��ɫ
	for (unsigned int i = 0; i < 2; i++) {
		glBindTexture(GL_TEXTURE_2D, postEffectColorBuffer[i]);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 800, 600, 0, GL_RGB, GL_UNSIGNED_BYTE, NULL);
		// �����������ģʽΪ���Բ���
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// ����ǰ������󸽼ӵ���ǰ��֡�������
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0+i, GL_TEXTURE_2D, postEffectColorBuffer[i], 0);

	}
	// ���֡�����Ƿ�����,���������,��ӡ������Ϣ
	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "֡���岻����!!!" << std::endl;
	}

	GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };
	glDrawBuffers(2, attachments);
	// ���֡����
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
#pragma endregion

#pragma region ��ʼ����˹ģ��֡����

	glGenFramebuffers(2,blurFBO);
	glGenTextures(2,blurColorBuffer);
	for (unsigned int i = 0; i < 2;i++) {
		glBindFramebuffer(GL_FRAMEBUFFER,blurFBO[i]);
		glBindTexture(GL_TEXTURE_2D,blurColorBuffer[i]);

		// �����������ò���/����ģʽ
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,800,600,0,GL_RGB,GL_FLOAT,NULL);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

		// ����������ӽ���ǰ֡������
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_2D,blurColorBuffer[i],0);

	}

#pragma endregion


#pragma region ��ʼ����Ļ�ı���

	unsigned int screenQuadVBO;
	// ��ʼ����Ļ�ı���
	glGenVertexArrays(1,&screenQuadVAO);
	glBindVertexArray(screenQuadVAO);
	glGenBuffers(1,&screenQuadVBO);
	glBindBuffer(GL_ARRAY_BUFFER,screenQuadVBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(screenQuadVertices),screenQuadVertices,GL_STATIC_DRAW);

	// ���ò����ö�������
	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,5*sizeof(float),0);
	// uv
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3*sizeof(float)));

	// ���vao,vbo
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER,0);

#pragma endregion

	// ��ʼ������Shader��Image
	texture = std::make_shared<Image>("Image/sakura1.jpg");
	bloomShader = std::make_shared<Shader>("Shader/Sample16 Bloom/bloomV.glsl", "Shader/Sample16 Bloom/bloomF.glsl");
	postEffectShader = std::make_shared<Shader>("Shader/Sample16 Bloom/postEffectV.glsl", "Shader/Sample16 Bloom/postEffectF.glsl");
	blurShader = std::make_shared<Shader>("Shader/Sample16 Bloom/postEffectV.glsl", "Shader/Sample16 Bloom/blurF.glsl");
}

void Sample16::render() {

#pragma region ��Ⱦ��������ȡ��ɫ
	glBindFramebuffer(GL_FRAMEBUFFER, postEffectFBO);
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
	// �����ɫ�������Ȼ���
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	bloomShader->use();

	bloomShader->setTexture2D("MainTex", *texture, 0);

	// ��Ⱦ����
	glBindVertexArray(screenQuadVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
#pragma endregion

#pragma region ����ɫ������˹ģ��

	GLboolean horizontal = true,first_iteration = true;
	GLuint amount = 50;
	blurShader->use();
	// ÿ������һ�θ�˹ģ��,����һ��ͼ���е���5�εĸ�˹ģ������
	for (unsigned int i = 0; i < amount; i++) {
		glBindFramebuffer(GL_FRAMEBUFFER,blurFBO[horizontal]);
		blurShader->setBool("horizontal",horizontal);
		// ���õ�ǰҪ����ģ����ͼ��,
		// ����ǵ�һ�ε���,��ô��ǰҪģ����ͼ��Ϊ��Ⱦ����ʱ�õ�����ɫͼ
		// ������ǵ�һ�ε���,��ô��ǰҪ��Ⱦ��ͼ��Ϊ��һ�ε���ʱ�õ���ͼ��
		glBindTexture(GL_TEXTURE_2D,first_iteration?postEffectColorBuffer[1]:blurColorBuffer[!horizontal]);
		blurShader->setInt("image",0);

		glBindVertexArray(screenQuadVAO);
		glDrawArrays(GL_TRIANGLES,0,6);

		horizontal = !horizontal;
		if (first_iteration) first_iteration = false;
	}

	// ���
	glBindFramebuffer(GL_FRAMEBUFFER,0);
#pragma endregion


	// ��Ĭ��֡����
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

	// ��Ⱦ������
	glDrawArrays(GL_TRIANGLES, 0, 6);

	// ���
	glBindVertexArray(0);
}

void Sample16::onDestory() {}

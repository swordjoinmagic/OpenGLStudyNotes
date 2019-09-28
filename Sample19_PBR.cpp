#include "Sample19_PBR.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "stb_image.h"

void renderSphere();

// �ĸ����Դ
glm::vec3 lightPostions[] = {
	glm::vec3(-10.0f,10.0f,10.0f),
	glm::vec3(10.0f,10.0f,10.0f),
	glm::vec3(-10.0f,-10.0f,10.0f),
	glm::vec3(10.0f,-10.0f,10.0f),
};
// Ӧ��hdr�Ĺ�Դ
glm::vec3 lightColors[] = {
	glm::vec3(300.0f,300.0f,300.0f),
	glm::vec3(300.0f,300.0f,300.0f),
	glm::vec3(300.0f,300.0f,300.0f),
	glm::vec3(300.0f,300.0f,300.0f)
};

// 7*7�������ʾ����
int rows = 7, cols = 7;
float spacing = 2.5;

// ����6���۲��������Ⱦ��������ͼ(ͶӰ������ҪΪ90��)
glm::mat4 captureProjection = glm::perspective(glm::radians(90.0f), 1.0f, 0.1f, 10.0f);
glm::mat4 captureViews[] = {
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(-1.0f,  0.0f,  0.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  1.0f,  0.0f), glm::vec3(0.0f,  0.0f,  1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, -1.0f,  0.0f), glm::vec3(0.0f,  0.0f, -1.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f,  1.0f), glm::vec3(0.0f, -1.0f,  0.0f)),
   glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f,  0.0f, -1.0f), glm::vec3(0.0f, -1.0f,  0.0f))
};

void Sample19::init() {
	// ��ʼ��Shader��Model
	sphereModel = std::make_shared<Model>("mesh/sphere.obj");
	cubeModel = std::make_shared<Cube>();
	pbrShader = std::make_shared<Shader>("Shader/Sample19 PBR/PBRV.glsl","Shader/Sample19 PBR/PBRF.glsl");
	skyBoxShader = std::make_shared<Shader>("Shader/Sample19 PBR/skyBoxV.glsl", "Shader/Sample19 PBR/skyBoxF.glsl");
	cubeMapShader = std::make_shared<Shader>("Shader/Sample19 PBR/createCubeMapWithHDRV.glsl", "Shader/Sample19 PBR/createCubeMapWithHDRF.glsl");
	irradianceShader = std::make_shared<Shader>("Shader/Sample19 PBR/createCubeMapWithHDRV.glsl", "Shader/Sample19 PBR/createLightMapF.glsl");

	pbrShader->use();
	pbrShader->setFloat3("albedo",0.5f,0.0f,0.0f);
	pbrShader->setFloat("ao",1.0f);

	loadHDR();
	// ��ʼ������ӳ����ͼ
	initCubeMapWithHDR();
	// ��ʼ�����ն���ͼ
	createIrradianceMap();

	// ��Ⱦhdr��������ͼ
	createCubeMapWithHDR();

	// ���ݻ�����ͼ,Ԥ�������õ����ն���ͼ
	renderIrradianceMap();
}

void Sample19::render() {

#pragma region ��ȾPBR��������
	glEnable(GL_DEPTH_TEST);
	pbrShader->use();

	pbrShader->setCubeMap("irradianceMap",irradianceMap,0);

	// �����ĸ���Դ��λ��
	for (unsigned int i = 0; i < 4; i++) {
		pbrShader->setFloat3("lightPos[" + std::to_string(i) + "]", lightPostions[i].x, lightPostions[i].y, lightPostions[i].z);
		pbrShader->setFloat3("lightColor[" + std::to_string(i) + "]", lightColors[i].r, lightColors[i].g, lightColors[i].b);

	}

	// ����vp����
	pbrShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	pbrShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	// ���ù۲��
	pbrShader->setFloat3("viewPos", camera->pos.x, camera->pos.y, camera->pos.z);

	glm::mat4 model(1.0f);
	// 7*7������,�������Ҵֲڶ�����,���ϵ��½���������
	for (int row = 0; row < rows; row++) {
		pbrShader->setFloat("metallic", (float)row / (float)rows);
		for (int col = 0; col < cols; col++) {
			pbrShader->setFloat("roughness", glm::clamp((float)col / (float)cols, 0.05f, 1.0f));

			model = glm::mat4(1.0f);
			model = glm::translate(model, glm::vec3(
				(col - (cols / 2)) * spacing,
				(row - (rows / 2)) * spacing,
				0.0f)
			);
			pbrShader->setMatrix4x4("model", glm::value_ptr(model));

			//sphereModel->Draw(*pbrShader);
			renderSphere();
		}
	}
#pragma endregion

	// ��Ⱦ��պ�
	renderSkyBox();

}

void Sample19::onDestory() {}

unsigned int sphereVAO = 0;
unsigned int indexCount;
void renderSphere() {
	if (sphereVAO == 0) {
		glGenVertexArrays(1, &sphereVAO);

		unsigned int vbo, ebo;
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		std::vector<glm::vec3> positions;
		std::vector<glm::vec2> uv;
		std::vector<glm::vec3> normals;
		std::vector<unsigned int> indices;

		const unsigned int X_SEGMENTS = 64;
		const unsigned int Y_SEGMENTS = 64;
		const float PI = 3.14159265359;
		for (unsigned int y = 0; y <= Y_SEGMENTS; ++y) {
			for (unsigned int x = 0; x <= X_SEGMENTS; ++x) {
				float xSegment = (float)x / (float)X_SEGMENTS;
				float ySegment = (float)y / (float)Y_SEGMENTS;
				float xPos = std::cos(xSegment * 2.0f * PI) * std::sin(ySegment * PI);
				float yPos = std::cos(ySegment * PI);
				float zPos = std::sin(xSegment * 2.0f * PI) * std::sin(ySegment * PI);

				positions.push_back(glm::vec3(xPos, yPos, zPos));
				uv.push_back(glm::vec2(xSegment, ySegment));
				normals.push_back(glm::vec3(xPos, yPos, zPos));
			}
		}

		bool oddRow = false;
		for (int y = 0; y < Y_SEGMENTS; ++y) {
			if (!oddRow) // even rows: y == 0, y == 2; and so on
			{
				for (int x = 0; x <= X_SEGMENTS; ++x) {
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
				}
			} else {
				for (int x = X_SEGMENTS; x >= 0; --x) {
					indices.push_back((y + 1) * (X_SEGMENTS + 1) + x);
					indices.push_back(y       * (X_SEGMENTS + 1) + x);
				}
			}
			oddRow = !oddRow;
		}
		indexCount = indices.size();

		std::vector<float> data;
		for (int i = 0; i < positions.size(); ++i) {
			data.push_back(positions[i].x);
			data.push_back(positions[i].y);
			data.push_back(positions[i].z);
			if (uv.size() > 0) {
				data.push_back(uv[i].x);
				data.push_back(uv[i].y);
			}
			if (normals.size() > 0) {
				data.push_back(normals[i].x);
				data.push_back(normals[i].y);
				data.push_back(normals[i].z);
			}
		}
		glBindVertexArray(sphereVAO);
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, data.size() * sizeof(float), &data[0], GL_STATIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);
		float stride = (3 + 2 + 3) * sizeof(float);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(5 * sizeof(float)));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
	}

	glBindVertexArray(sphereVAO);
	glDrawElements(GL_TRIANGLE_STRIP, indexCount, GL_UNSIGNED_INT, 0);
}


void Sample19::loadHDR() {
	stbi_set_flip_vertically_on_load(true);

	int width, height, nrComponents;
	float *data = stbi_loadf("Image/newport_loft.hdr",&width,&height,&nrComponents,0);
	if (data) {
		glGenTextures(1,&hdrTexture);
		glBindTexture(GL_TEXTURE_2D,hdrTexture);
		// �����������ͼ����Ϊ16λ������(���ڴ洢hdr�г���1.0����ֵ)
		glTexImage2D(GL_TEXTURE_2D,0,GL_RGB16F,width,height,0,GL_RGB,GL_FLOAT,data);

		// �����������ģʽ
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	} else {
		std::cout << "��ȡHDRͼƬ����!" << std::endl;
	}
}

void Sample19::initCubeMapWithHDR() {

	// ��ʼ��cubemap
	glGenTextures(1,&hdrCubeMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP,hdrCubeMap);
	for (unsigned int i = 0; i < 6; i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,512,512,0,GL_RGB,GL_FLOAT,nullptr);
	}
	// �����������/����ģʽ
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// ��ʼ��֡����
	glGenFramebuffers(1,&captureFBO);
	glGenRenderbuffers(1,&captureRBO);

	glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,512,512);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER,GL_DEPTH_ATTACHMENT,GL_RENDERBUFFER,captureRBO);

	// �ָ�Ĭ��֡����
	glBindFramebuffer(GL_FRAMEBUFFER,0);
}

void Sample19::createCubeMapWithHDR() {

	cubeMapShader->use();
	cubeMapShader->setInt("equirectangularMap",0);
	cubeMapShader->setMatrix4x4("projection",glm::value_ptr(captureProjection));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D,hdrTexture);

	glViewport(0,0,512,512);
	glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
	glEnable(GL_DEPTH_TEST);
	for (unsigned int i = 0; i < 6;i++) {
		// ��6����Ⱦ����ֱ�洢��cubeMap��6������
		cubeMapShader->setMatrix4x4("view",glm::value_ptr(captureViews[i]));
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,hdrCubeMap,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeModel->Draw(*cubeMapShader);
	}
	// �ָ�
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	
}

// ��Ⱦ��պ�,�������Ⱦ˳��Ϊ�����Ⱦ
void Sample19::renderSkyBox() {
	// ������Ȳ���,������Ȳ��Ժ�����ΪС�ڵ���
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	glViewport(0,0,800,600);
	glBindFramebuffer(GL_FRAMEBUFFER,0);

	skyBoxShader->use();
	skyBoxShader->setCubeMap("environmentMap",hdrCubeMap,0);
	skyBoxShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));
	skyBoxShader->setMatrix4x4("view",glm::value_ptr(camera->getViewMatrix()));

	cubeModel->Draw(*skyBoxShader);

	// �ָ�
	glDepthFunc(GL_LESS);
}

void Sample19::createIrradianceMap() {
	glGenTextures(1,&irradianceMap);
	glBindTexture(GL_TEXTURE_CUBE_MAP,irradianceMap);
	for (unsigned int i = 0; i < 6;i++) {
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,0,GL_RGB16F,32,32,0,GL_RGB,GL_FLOAT,nullptr);
	}
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
}

void Sample19::renderIrradianceMap() {
	// ��֡����,���Խϵ͵ķֱ��ʴ洢��Ļ��ͼ
	glBindFramebuffer(GL_FRAMEBUFFER, captureFBO);
	glBindRenderbuffer(GL_RENDERBUFFER,captureRBO);
	glRenderbufferStorage(GL_RENDERBUFFER,GL_DEPTH_COMPONENT24,32,32);

	irradianceShader->use();
	irradianceShader->setInt("environmentMap",0);
	irradianceShader->setMatrix4x4("projection",glm::value_ptr(captureProjection));
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP,hdrCubeMap);

	glViewport(0,0,32,32);
	glBindFramebuffer(GL_FRAMEBUFFER,captureFBO);
	for (unsigned int i = 0; i < 6; i++) {
		irradianceShader->setMatrix4x4("view",glm::value_ptr(captureViews[i]));
		// ���õ�ǰ��Ⱦ��ͼ���ӵ��ĸ�texture��
		glFramebufferTexture2D(GL_FRAMEBUFFER,GL_COLOR_ATTACHMENT0,GL_TEXTURE_CUBE_MAP_POSITIVE_X+i,irradianceMap,0);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		cubeModel->Draw(*irradianceShader);
	}

	// �ָ�
	glBindFramebuffer(GL_FRAMEBUFFER,0);
	glViewport(0, 0, 800, 600);

}
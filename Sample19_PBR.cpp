#include "Sample19_PBR.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void renderSphere();

// 四个点光源
glm::vec3 lightPostions[] = {
	glm::vec3(-10.0f,10.0f,10.0f),
	glm::vec3(10.0f,10.0f,10.0f),
	glm::vec3(-10.0f,-10.0f,10.0f),
	glm::vec3(10.0f,-10.0f,10.0f),
};
// 应用hdr的光源
glm::vec3 lightColors[] = {
	glm::vec3(300.0f,300.0f,300.0f),
	glm::vec3(300.0f,300.0f,300.0f),
	glm::vec3(300.0f,300.0f,300.0f),
	glm::vec3(300.0f,300.0f,300.0f)
};

// 7*7个球体表示材质
int rows = 7, cols = 7;
float spacing = 2.5;

void Sample19::init() {
	// 初始化Shader和Model
	sphereModel = std::make_shared<Model>("mesh/sphere.obj");
	pbrShader = std::make_shared<Shader>("Shader/Sample19 PBR/PBRV.glsl","Shader/Sample19 PBR/PBRF.glsl");

	pbrShader->use();
	pbrShader->setFloat3("albedo",0.5f,0.0f,0.0f);
	pbrShader->setFloat("ao",1.0f);
}

void Sample19::render() {
	glEnable(GL_DEPTH_TEST);
	pbrShader->use();

	// 设置四个光源的位置
	for (unsigned int i = 0; i < 4;i++) {
		pbrShader->setFloat3("lightPos["+std::to_string(i)+"]",lightPostions[i].x, lightPostions[i].y, lightPostions[i].z);
		pbrShader->setFloat3("lightColor["+std::to_string(i)+"]",lightColors[i].r, lightColors[i].g, lightColors[i].b);

	}

	// 设置vp矩阵
	pbrShader->setMatrix4x4("view",glm::value_ptr(camera->getViewMatrix()));
	pbrShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));
	// 设置观察点
	pbrShader->setFloat3("viewPos",camera->pos.x, camera->pos.y, camera->pos.z);

	glm::mat4 model(1.0f);
	// 7*7个球形,从左向右粗糙度增大,从上到下金属度增大
	for (int row = 0; row < rows;row++) {
		pbrShader->setFloat("metallic",(float)row/(float)rows);
		for (int col = 0; col < cols;col++) {
			pbrShader->setFloat("roughness",glm::clamp((float)col/(float)cols,0.05f,1.0f));

			model = glm::mat4(1.0f);
			model = glm::translate(model,glm::vec3(
					(col - (cols/2)) * spacing,
					(row - (rows/2)) * spacing,
					0.0f)
			);
			pbrShader->setMatrix4x4("model",glm::value_ptr(model));

			//sphereModel->Draw(*pbrShader);
			renderSphere();
		}
	}
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

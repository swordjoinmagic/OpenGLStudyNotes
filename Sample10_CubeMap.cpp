#include "Sample10_CubeMap.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const float skyboxVertices[] = {
	// positions          
	-1.0f,  1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,

	-1.0f, -1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	-1.0f, -1.0f,  1.0f,

	-1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f, -1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f,  1.0f,
	-1.0f,  1.0f, -1.0f,

	-1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f
};
std::vector<std::string> path
{
	"Image/skybox/right.jpg",
	"Image/skybox/left.jpg",
	"Image/skybox/top.jpg",
	"Image/skybox/bottom.jpg",
	"Image/skybox/front.jpg",
	"Image/skybox/back.jpg"
};

void Sample10::init() {

	// ��ʼ��obj��shader������
	objShader = std::make_shared<Shader>("Shader/Sample9 PostEffect/objVertex.glsl", "Shader/Sample9 PostEffect/objFrag.glsl");
	objTexture = std::make_shared<Image>("Image/wall.jpg");
	objModel = std::make_shared<Cube>();

	reflectShader = std::make_shared<Shader>("Shader/Sample10 CubeMap/reflectV.glsl", "Shader/Sample10 CubeMap/reflectF.glsl");
	refractShader = std::make_shared<Shader>("Shader/Sample10 CubeMap/reflectV.glsl", "Shader/Sample10 CubeMap/refracF.glsl");

	// ��ʼ��cubeMap
	cubemap = std::make_shared<CubeMap>(path);
	skyBoxShader = std::make_shared<Shader>("Shader/Sample10 CubeMap/skyboxV.glsl", "Shader/Sample10 CubeMap/skyboxF.glsl");

	// ��ʼ����պе�mesh
	unsigned int VBO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);

	// ���ö�������
	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

	// �ָ�Ĭ�ϰ󶨶���
	glBindVertexArray(0);
}

void Sample10::render() {
	glBindVertexArray(VAO);

	// ������Ȳ���
	glEnable(GL_DEPTH_TEST);
	// �������д��(��֤��պв������Ȼ������Ӱ��)
	glDepthMask(GL_FALSE);

	// ������պ�
	skyBoxShader->use();
	// ���ù۲��ͶӰ����
	skyBoxShader->setMatrix4x4("view",glm::value_ptr(glm::mat4(glm::mat3(camera->getViewMatrix()))));
	skyBoxShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));
	// ������պ�����
	skyBoxShader->setCubeMap("skybox",*cubemap,0);

	glDrawArrays(GL_TRIANGLES,0,36);

	// �ָ����д��
	glDepthMask(GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	objShader->use();
	// ���ù۲��ͶӰ����
	objShader->setMatrix4x4("model",glm::value_ptr(glm::mat4(1.0f)));
	objShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	objShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	objShader->setTexture2D("MainTex",*objTexture,0);
	objModel->Draw(*objShader);

	// ���ƻ���ӳ���������
	reflectShader->use();
	// ����mvp
	glm::mat4 model(1.0f);
	model = glm::translate(model,glm::vec3(1,1,1));
	reflectShader->setMatrix4x4("model", glm::value_ptr(model));
	reflectShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	reflectShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	reflectShader->setFloat3("viewPos",camera->pos.x, camera->pos.y, camera->pos.z);
	reflectShader->setCubeMap("skybox", *cubemap, 0);

	objModel->Draw(*reflectShader);
	
	// ���������������
	refractShader->use();
	// ����mvp
	glm::mat4 model2(1.0f);
	model2 = glm::translate(model2, glm::vec3(-1, -1, 1));
	refractShader->setMatrix4x4("model", glm::value_ptr(model2));
	refractShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	refractShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	refractShader->setFloat3("viewPos", camera->pos.x, camera->pos.y, camera->pos.z);
	refractShader->setCubeMap("skybox", *cubemap, 0);

	objModel->Draw(*refractShader);
}

void Sample10::onDestory() {}
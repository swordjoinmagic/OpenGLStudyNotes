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

	// 初始化obj的shader和纹理
	objShader = std::make_shared<Shader>("Shader/Sample9 PostEffect/objVertex.glsl", "Shader/Sample9 PostEffect/objFrag.glsl");
	objTexture = std::make_shared<Image>("Image/wall.jpg");
	objModel = std::make_shared<Cube>();

	reflectShader = std::make_shared<Shader>("Shader/Sample10 CubeMap/reflectV.glsl", "Shader/Sample10 CubeMap/reflectF.glsl");
	refractShader = std::make_shared<Shader>("Shader/Sample10 CubeMap/reflectV.glsl", "Shader/Sample10 CubeMap/refracF.glsl");

	// 初始化cubeMap
	cubemap = std::make_shared<CubeMap>(path);
	skyBoxShader = std::make_shared<Shader>("Shader/Sample10 CubeMap/skyboxV.glsl", "Shader/Sample10 CubeMap/skyboxF.glsl");

	// 初始化天空盒的mesh
	unsigned int VBO;
	glGenVertexArrays(1,&VAO);
	glGenBuffers(1,&VBO);
	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,sizeof(skyboxVertices),skyboxVertices,GL_STATIC_DRAW);

	// 启用顶点数据
	// pos
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,3*sizeof(float),0);

	// 恢复默认绑定对象
	glBindVertexArray(0);
}

void Sample10::render() {
	glBindVertexArray(VAO);

	// 开启深度测试
	glEnable(GL_DEPTH_TEST);
	// 禁用深度写入(保证天空盒不会对深度缓冲产生影响)
	glDepthMask(GL_FALSE);

	// 绘制天空盒
	skyBoxShader->use();
	// 设置观察和投影矩阵
	skyBoxShader->setMatrix4x4("view",glm::value_ptr(glm::mat4(glm::mat3(camera->getViewMatrix()))));
	skyBoxShader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));
	// 设置天空盒纹理
	skyBoxShader->setCubeMap("skybox",*cubemap,0);

	glDrawArrays(GL_TRIANGLES,0,36);

	// 恢复深度写入
	glDepthMask(GL_TRUE);

	glEnable(GL_DEPTH_TEST);
	objShader->use();
	// 设置观察和投影矩阵
	objShader->setMatrix4x4("model",glm::value_ptr(glm::mat4(1.0f)));
	objShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	objShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	objShader->setTexture2D("MainTex",*objTexture,0);
	objModel->Draw(*objShader);

	// 绘制环境映射的立方体
	reflectShader->use();
	// 设置mvp
	glm::mat4 model(1.0f);
	model = glm::translate(model,glm::vec3(1,1,1));
	reflectShader->setMatrix4x4("model", glm::value_ptr(model));
	reflectShader->setMatrix4x4("view", glm::value_ptr(camera->getViewMatrix()));
	reflectShader->setMatrix4x4("projection", glm::value_ptr(camera->getProjectionMatrix()));
	reflectShader->setFloat3("viewPos",camera->pos.x, camera->pos.y, camera->pos.z);
	reflectShader->setCubeMap("skybox", *cubemap, 0);

	objModel->Draw(*reflectShader);
	
	// 设置折射的立方体
	refractShader->use();
	// 设置mvp
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
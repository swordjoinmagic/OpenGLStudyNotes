#include "Sample5_AssimpTest.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

void Sample5::init() {	
	// º”‘ÿmesh
	model = std::make_shared<Model>("mesh/nanosuit/nanosuit.obj");
	// º”‘ÿshader
	shader = std::make_shared<Shader>("Shader/Sample5 Assimp/testVertex.glsl", "Shader/Sample5 Assimp/testFrag.glsl");
	image = std::make_shared<Image>("Image/container.jpg");
}

void Sample5::render() {
	shader->use();

	shader->setTexture2D("MainTex",*image,0);

	glEnable(GL_DEPTH_TEST);

	// …Ë÷√mvpæÿ’Û
	glm::mat4 mMatrix(1.0);
	mMatrix = glm::scale(mMatrix,glm::vec3(1,1,1)*0.2f);
	shader->setMatrix4x4("model",glm::value_ptr(mMatrix));
	shader->setMatrix4x4("view",glm::value_ptr(camera->getViewMatrix()));
	shader->setMatrix4x4("projection",glm::value_ptr(camera->getProjectionMatrix()));

	model->Draw(*shader);
}

void Sample5::onDestory() {
	
}
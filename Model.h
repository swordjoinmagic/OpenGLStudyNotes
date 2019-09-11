#pragma once
#ifndef MODEL
#define MODEL

#include <vector>
#include <string>
#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Shader.h"

using namespace SJM;

/*
	一个Model包含了多个Mesh,Mesh(网格)是GL渲染的最小单位,
	一个完整的模型(Model)由多个Mesh(网格)组成,
	如一个人由手臂/大腿/头部等构成
*/
class Model {
public:
	Model(const char* path);
	Model(std::vector<Mesh> meshes) { this->meshes = meshes; }
	Model() {};
	void Draw(Shader shader);
protected:
	// 模型数据
	std::vector<Mesh> meshes;
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode *node,const aiScene *scene);
	Mesh processMesh(aiMesh *mesh,const aiScene *scene);
	std::vector<Image> loadMaterialTextures(aiMaterial *mat,aiTextureType type,std::string typeName);
};

#endif // !MODEL

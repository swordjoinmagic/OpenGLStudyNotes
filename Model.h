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
	һ��Model�����˶��Mesh,Mesh(����)��GL��Ⱦ����С��λ,
	һ��������ģ��(Model)�ɶ��Mesh(����)���,
	��һ�������ֱ�/����/ͷ���ȹ���
*/
class Model {
public:
	Model(const char* path);
	Model(std::vector<Mesh> meshes) { this->meshes = meshes; }
	Model() {};
	void Draw(Shader shader);
protected:
	// ģ������
	std::vector<Mesh> meshes;
	std::string directory;
	void loadModel(std::string path);
	void processNode(aiNode *node,const aiScene *scene);
	Mesh processMesh(aiMesh *mesh,const aiScene *scene);
	std::vector<Image> loadMaterialTextures(aiMaterial *mat,aiTextureType type,std::string typeName);
};

#endif // !MODEL

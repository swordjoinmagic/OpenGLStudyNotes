#include "Model.h"
#include "Vertex.h"
#include "Image.h"
#include <glm/glm.hpp>

using namespace SJM;

Model::Model(const char* path) {
	loadModel(path);
}

void Model::loadModel(std::string path) {
	Assimp::Importer importer;

	// ����ģ�ͳ���,��ָ����ģ���������湹��(��������������ת��)������uv
	const aiScene *scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// ��������ʧ�ܵ����
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		std::cout << "Assimp���س���ʧ��:" << importer.GetErrorString() << std::endl;
	
	// ȥ��·�����һ��/
	directory = path.substr(0,path.find_last_of('/'));

	// �ݹ鴦���������нڵ�
	processNode(scene->mRootNode,scene);
}

void Model::processNode(aiNode *node,const aiScene *scene) {

	// ����ڵ������е�����
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh,scene));
	}

	// �Ե�ǰ�ڵ���ӽڵ��ظ���������
	for (unsigned int i = 0; i < node->mNumChildren;i++) {
		processNode(node->mChildren[i],scene);
	}
}

Mesh Model::processMesh(aiMesh *mesh, const aiScene *scene) {
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Image> textures;

	for (unsigned int i = 0; i < mesh->mNumVertices; i++) {
		Vertex vertex;
		// ������λ��/����/uv

		//pos
		glm::vec3 vector;
		vector.x = mesh->mVertices[i].x;
		vector.y = mesh->mVertices[i].y;
		vector.z = mesh->mVertices[i].z;
		vertex.position = vector;

		// normal
		vector.x = mesh->mNormals[i].x;
		vector.y = mesh->mNormals[i].y;
		vector.z = mesh->mNormals[i].z;
		vertex.normal = vector;

		// uv,�ж������Ƿ�����������
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			// ֻȡ��һ����������
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = vec;
		}
		else {
			vertex.texcoords = glm::vec2(0.0f, 0.0f);
		}

		// ����
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;

		vertices.push_back(vertex);
	}

	// ��������
	for (unsigned int i = 0; i<mesh->mNumFaces;i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	Mesh result(vertices,indices);

	// �������
	if (mesh->mMaterialIndex >= 0) {
		aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
		for (int i = 0; i < material->GetTextureCount(aiTextureType_DIFFUSE) && i<1; i++) {
			aiString str;
			material->GetTexture(aiTextureType_DIFFUSE,i,&str);
			
			std::string path = directory + "/" + std::string(str.C_Str());

			Image texture(path.c_str());

			result.textures.push_back(texture);
		}
	}

	return result;
}

void Model::Draw(Shader shader) {
	for (Mesh mesh : meshes) {
		mesh.Draw(shader);
	}
}
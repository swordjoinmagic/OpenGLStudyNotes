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

	// 加载模型场景,并指定该模型由三角面构成(非三角面的情况会转换)并生成uv
	const aiScene *scene = importer.ReadFile(path,aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

	// 场景加载失败的情况
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) 
		std::cout << "Assimp加载场景失败:" << importer.GetErrorString() << std::endl;
	
	// 去掉路径最后一个/
	directory = path.substr(0,path.find_last_of('/'));

	// 递归处理场景中所有节点
	processNode(scene->mRootNode,scene);
}

void Model::processNode(aiNode *node,const aiScene *scene) {

	// 处理节点中所有的网格
	for (unsigned int i = 0; i < node->mNumMeshes; i++) {
		aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(processMesh(mesh,scene));
	}

	// 对当前节点的子节点重复上述过程
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
		// 处理顶点位置/法线/uv

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

		// uv,判断网格是否有纹理坐标
		if (mesh->mTextureCoords[0]) {
			glm::vec2 vec;
			// 只取第一套纹理坐标
			vec.x = mesh->mTextureCoords[0][i].x;
			vec.y = mesh->mTextureCoords[0][i].y;
			vertex.texcoords = vec;
		}
		else {
			vertex.texcoords = glm::vec2(0.0f, 0.0f);
		}

		// 切线
		vector.x = mesh->mTangents[i].x;
		vector.y = mesh->mTangents[i].y;
		vector.z = mesh->mTangents[i].z;
		vertex.tangent = vector;

		vertices.push_back(vertex);
	}

	// 处理索引
	for (unsigned int i = 0; i<mesh->mNumFaces;i++) {
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
			indices.push_back(face.mIndices[j]);
	}

	Mesh result(vertices,indices);

	// 处理材质
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
#pragma once
#ifndef MESH
#define MESH

#include <vector>
#include "Vertex.h"
#include "Image.h"
#include "Shader.h"

using namespace SJM;

class Mesh {
public:
	// ��������
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Image> textures;
	Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices);

	void Draw(Shader shader);
private:
	unsigned int VAO, VBO, EBO;
	// �������еĶ���/������������,����Mesh����
	void setupMesh();
};

#endif // !MESH

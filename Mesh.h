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
	// 网格数据
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<Image> textures;
	Mesh(std::vector<Vertex> vertices,std::vector<unsigned int> indices);

	void Draw(Shader shader);
private:
	unsigned int VAO, VBO, EBO;
	// 根据已有的顶点/顶点索引序列,建立Mesh网格
	void setupMesh();
};

#endif // !MESH

#include "Mesh.h"

Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
}

void Mesh::setupMesh() {
	// 生成顶点数组对象
	glGenVertexArrays(1,&VAO);
	// 生成顶点缓冲和索引缓冲对象
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);

	// 绑定顶点数组对象
	glBindVertexArray(VAO);

	// 绑定顶点缓冲对象并向其输入数据
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);

	// 绑定顶点索引缓冲,并向其输入数据
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);

	// 设置并启用顶点属性

	// pos
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void *)0);
	glEnableVertexAttribArray(0);
	
	// Noraml
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void *)offsetof(Vertex,normal) );
	glEnableVertexAttribArray(1);

	// uv
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,texcoords));
	glEnableVertexAttribArray(2);

	// 清除绑定
	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader) {

	// 绑定顶点数组对象
	glBindVertexArray(VAO);
	// 使用Shader
	shader.use();

	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
	
	// 清除绑定
	glBindVertexArray(0);
}
#include "Mesh.h"

Mesh::Mesh(
	std::vector<Vertex> vertices,
	std::vector<unsigned int> indices) {
	this->vertices = vertices;
	this->indices = indices;

	setupMesh();
}

void Mesh::setupMesh() {
	// ���ɶ����������
	glGenVertexArrays(1,&VAO);
	// ���ɶ��㻺��������������
	glGenBuffers(1,&VBO);
	glGenBuffers(1,&EBO);

	// �󶨶����������
	glBindVertexArray(VAO);

	// �󶨶��㻺�����������������
	glBindBuffer(GL_ARRAY_BUFFER,VBO);
	glBufferData(GL_ARRAY_BUFFER,vertices.size() * sizeof(Vertex),&vertices[0],GL_STATIC_DRAW);

	// �󶨶�����������,��������������
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,indices.size() * sizeof(unsigned int),&indices[0],GL_STATIC_DRAW);

	// ���ò����ö�������

	// pos
	glVertexAttribPointer(0,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void *)0);
	glEnableVertexAttribArray(0);
	
	// Noraml
	glVertexAttribPointer(1,3,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void *)offsetof(Vertex,normal) );
	glEnableVertexAttribArray(1);

	// uv
	glVertexAttribPointer(2,2,GL_FLOAT,GL_FALSE,sizeof(Vertex),(void*)offsetof(Vertex,texcoords));
	glEnableVertexAttribArray(2);

	// �����
	glBindVertexArray(0);
}

void Mesh::Draw(Shader shader) {

	// �󶨶����������
	glBindVertexArray(VAO);
	// ʹ��Shader
	shader.use();

	glDrawElements(GL_TRIANGLES,indices.size(),GL_UNSIGNED_INT,0);
	
	// �����
	glBindVertexArray(0);
}
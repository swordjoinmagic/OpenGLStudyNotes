#pragma once
#ifndef VERTEX
#define VERTEX

#include <glm/glm.hpp>

struct Vertex{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec2 texcoords;
};

#endif // !VERTEX

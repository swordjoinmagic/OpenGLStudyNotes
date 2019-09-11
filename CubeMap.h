#pragma once
#ifndef CUBEMAP
#define CUBEMAP

#include "main.h"
#include <vector>
#include <string>

namespace SJM {

	class CubeMap {		
	public:
		// 该cubeMap对象在OpenGL中的ID
		unsigned int textureID;		
		// 使用一系列的纹理的路径来初始化CubeMap,
		// 该vector需按照以下顺序:
		// 右/左/上/下/后/前
		CubeMap(std::vector<std::string> path);

	};

};


#endif // !CUBEMAP

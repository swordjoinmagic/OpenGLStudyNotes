#pragma once
#ifndef CUBEMAP
#define CUBEMAP

#include "main.h"
#include <vector>
#include <string>

namespace SJM {

	class CubeMap {		
	public:
		// ��cubeMap������OpenGL�е�ID
		unsigned int textureID;		
		// ʹ��һϵ�е������·������ʼ��CubeMap,
		// ��vector�谴������˳��:
		// ��/��/��/��/��/ǰ
		CubeMap(std::vector<std::string> path);

	};

};


#endif // !CUBEMAP

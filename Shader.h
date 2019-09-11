#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "main.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "Image.h"
#include "CubeMap.h"

class Shader {
public:
	// 着色器程序ID
	unsigned int ID;
	// 从文件路径中获得着色器源代码构造着色器
	Shader(const GLchar* vertexPath,const GLchar* fragmentPath);	
	Shader() {}
	// 使用/激活程序
	void use();

	// 设置uniform的函数
	void setBool(const std::string &name,bool value) const;
	void setInt(const std::string &name,int value) const;
	void setFloat(const std::string &name,float value) const;

	// 设置纹理
	void setTexture2D(const std::string &name,const SJM::Image image,int textureUnit);
	void setCubeMap(const std::string &name,const SJM::CubeMap,int textureUnit);

	// 设置矩阵(变换)
	void setMatrix4x4(const std::string &name,const float* value);

	// 设置向量
	void setFloat3(const std::string &name, const float x, const float y, const float z);
	void setFloat4(const std::string &name, const float x, const float y, const float z,const float w);
private:
	std::string getSourceCode(const char* codePath);
	void checkCreateShaderError(int shaderID, const char* extraLog);
	void checkLinkShaderError();
};

#endif // !SHADER_H

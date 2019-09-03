#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "main.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

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
private:
	std::string getSourceCode(const char* codePath);
	void checkCreateShaderError(int shaderID);
	void checkLinkShaderError();
};

#endif // !SHADER_H

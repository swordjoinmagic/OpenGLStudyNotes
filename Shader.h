#pragma once
#ifndef SHADER_H
#define SHADER_H

#include "main.h"
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "Image.h"

class Shader {
public:
	// ��ɫ������ID
	unsigned int ID;
	// ���ļ�·���л����ɫ��Դ���빹����ɫ��
	Shader(const GLchar* vertexPath,const GLchar* fragmentPath);
	Shader() {}
	// ʹ��/�������
	void use();

	// ����uniform�ĺ���
	void setBool(const std::string &name,bool value) const;
	void setInt(const std::string &name,int value) const;
	void setFloat(const std::string &name,float value) const;

	// ��������
	void setTexture2D(const std::string &name,const SJM::Image image,int textureUnit);

	// ���þ���(�任)
	void setMatrix4x4(const std::string &name,const float* value);
private:
	std::string getSourceCode(const char* codePath);
	void checkCreateShaderError(int shaderID);
	void checkLinkShaderError();
};

#endif // !SHADER_H
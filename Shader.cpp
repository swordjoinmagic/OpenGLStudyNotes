#include "Shader.h"

// 根据着色器代码的路径读取源码,返回const char*类型
std::string Shader::getSourceCode(const char* codePath) {
	std::string sourceCode;
	std::ifstream shaderFile;
	// 保证ifstream对象可以抛出异常
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// 打开文件
		shaderFile.open(codePath);

		// 创建字符流
		std::stringstream shaderStream;

		// 读取文件的缓冲内容到数据流中
		shaderStream << shaderFile.rdbuf();

		// 关闭文件处理器
		shaderFile.close();

		// 转换数据流到string
		sourceCode = shaderStream.str();

	}catch (std::ifstream::failure e) {
		std::cout << "在读取着色器源码时发生异常~" << std::endl;
	}

	return sourceCode;
}

// 检查顶点着色器的编译是否成功,如果失败,打印错误
void Shader::checkCreateShaderError(int shaderID) {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		// 获得错误信息
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "编译着色器出错\n" << infoLog << std::endl;
	}
}

void Shader::checkLinkShaderError() {
	int success;
	char infoLog[512];
	glGetProgramiv(ID,GL_LINK_STATUS,&success);
	if (!success) {
		glGetProgramInfoLog(ID,512,NULL,infoLog);
		std::cout<< "链接着色器时发生错误!\n" << infoLog <<std::endl;
	}
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath){

	// 将string类型转换为C语言内置字符串(即char*)
	std::string vertexCode = getSourceCode(vertexPath);
	std::string fragmentCode = getSourceCode(fragmentPath);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// 编译着色器
	unsigned int vertex, fragment;
	// 创建顶点着色器
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex,1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// 检查并打印编译错误
	checkCreateShaderError(vertex);

	// 创建片元着色器
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment,1,&fShaderCode,NULL);
	glCompileShader(fragment);
	// 检查并打印编译错误
	checkCreateShaderError(fragment);

	// 创建着色器程序(用于链接所有着色器)
	this->ID = glCreateProgram();
	glAttachShader(ID,vertex);
	glAttachShader(ID,fragment);
	glLinkProgram(ID);
	// 检查并打印链接错误
	checkLinkShaderError();

	// 删除着色器,因为已经链接进程序中了
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

// setter函数
void Shader::setBool(const std::string &name,bool value) const {
	glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}
void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

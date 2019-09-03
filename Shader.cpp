#include "Shader.h"

// ������ɫ�������·����ȡԴ��,����const char*����
std::string Shader::getSourceCode(const char* codePath) {
	std::string sourceCode;
	std::ifstream shaderFile;
	// ��֤ifstream��������׳��쳣
	shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
	try {
		// ���ļ�
		shaderFile.open(codePath);

		// �����ַ���
		std::stringstream shaderStream;

		// ��ȡ�ļ��Ļ������ݵ���������
		shaderStream << shaderFile.rdbuf();

		// �ر��ļ�������
		shaderFile.close();

		// ת����������string
		sourceCode = shaderStream.str();

	}catch (std::ifstream::failure e) {
		std::cout << "�ڶ�ȡ��ɫ��Դ��ʱ�����쳣~" << std::endl;
	}

	return sourceCode;
}

// ��鶥����ɫ���ı����Ƿ�ɹ�,���ʧ��,��ӡ����
void Shader::checkCreateShaderError(int shaderID) {
	int success;
	char infoLog[512];
	glGetShaderiv(shaderID, GL_COMPILE_STATUS, &success);
	if (!success) {
		// ��ô�����Ϣ
		glGetShaderInfoLog(shaderID, 512, NULL, infoLog);
		std::cout << "������ɫ������\n" << infoLog << std::endl;
	}
}

void Shader::checkLinkShaderError() {
	int success;
	char infoLog[512];
	glGetProgramiv(ID,GL_LINK_STATUS,&success);
	if (!success) {
		glGetProgramInfoLog(ID,512,NULL,infoLog);
		std::cout<< "������ɫ��ʱ��������!\n" << infoLog <<std::endl;
	}
}

Shader::Shader(const GLchar* vertexPath, const GLchar* fragmentPath){

	// ��string����ת��ΪC���������ַ���(��char*)
	std::string vertexCode = getSourceCode(vertexPath);
	std::string fragmentCode = getSourceCode(fragmentPath);

	const char* vShaderCode = vertexCode.c_str();
	const char* fShaderCode = fragmentCode.c_str();

	// ������ɫ��
	unsigned int vertex, fragment;
	// ����������ɫ��
	vertex = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertex,1, &vShaderCode, NULL);
	glCompileShader(vertex);
	// ��鲢��ӡ�������
	checkCreateShaderError(vertex);

	// ����ƬԪ��ɫ��
	fragment = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragment,1,&fShaderCode,NULL);
	glCompileShader(fragment);
	// ��鲢��ӡ�������
	checkCreateShaderError(fragment);

	// ������ɫ������(��������������ɫ��)
	this->ID = glCreateProgram();
	glAttachShader(ID,vertex);
	glAttachShader(ID,fragment);
	glLinkProgram(ID);
	// ��鲢��ӡ���Ӵ���
	checkLinkShaderError();

	// ɾ����ɫ��,��Ϊ�Ѿ����ӽ���������
	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

void Shader::use() {
	glUseProgram(ID);
}

// setter����
void Shader::setBool(const std::string &name,bool value) const {
	glUniform1i(glGetUniformLocation(ID,name.c_str()),(int)value);
}
void Shader::setInt(const std::string &name, int value) const {
	glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}
void Shader::setFloat(const std::string &name, float value) const {
	glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

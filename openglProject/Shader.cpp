#pragma once
#include "shader.h"
void Shader::shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;
    std::ifstream vShaderFile;
    std::ifstream fShaderFile;
    vShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try
    {
        vShaderFile.open(vertexPath);
        fShaderFile.open(fragmentPath);
        std::stringstream vShaderStream, fShaderStream;
        vShaderStream << vShaderFile.rdbuf();
        fShaderStream << fShaderFile.rdbuf();
        vShaderFile.close();
        fShaderFile.close();
        vertexCode = vShaderStream.str();
        fragmentCode = fShaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " << e.what() << std::endl;
    }
    const char* vShaderCode = vertexCode.c_str();
    const char* fShaderCode = fragmentCode.c_str();
    unsigned int vertex, fragment;
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vShaderCode, NULL);
    glCompileShader(vertex);
    checkCompileErrors(vertex, "VERTEX");
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fShaderCode, NULL);
    glCompileShader(fragment);
    checkCompileErrors(fragment, "FRAGMENT");
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    checkCompileErrors(ID, "PROGRAM");
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}
//启用着色器
void Shader::use()
{
    glUseProgram(ID);
}
int Shader::getUniformLocation(const char* name)
{
	std::map<string, int>::iterator pos;
	pos = uniformLocations.find(name);

	if (pos == uniformLocations.end())
	{
		uniformLocations[name] = glGetUniformLocation(ID, name);

	}

	return uniformLocations[name];
}
//绑定着色器关键字
void Shader::setUniform(const char* name, float x, float y, float z)
{
	GLint loc = getUniformLocation(name);
	glUniform3f(loc, x, y, z);
}

void Shader::setUniform(const char* name, const vec3& v)
{
	this->setUniform(name, v.x, v.y, v.z);
}

void Shader::setUniform(const char* name, const vec4& v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void Shader::setUniform(const char* name, const vec2& v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void Shader::setUniform(const char* name, const mat4& m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char* name, const mat3& m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void Shader::setUniform(const char* name, float val)
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, val);
}

void Shader::setUniform(const char* name, int val)
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, val);
}

void Shader::setUniform(const char* name, GLuint val)
{
	GLint loc = getUniformLocation(name);
	glUniform1ui(loc, val);
}

void Shader::setUniform(const char* name, bool val)
{
	int loc = getUniformLocation(name);
	glUniform1i(loc, val);
}
//关闭shader
void Shader::unuse()
{
    glUseProgram(0);
}
void Shader::checkCompileErrors(unsigned int shader, std::string type)
{
    int success;
    char infoLog[1024];
    if (type != "PROGRAM")
    {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::SHADER_COMPILATION_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
    else
    {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(shader, 1024, NULL, infoLog);
            std::cout << "ERROR::PROGRAM_LINKING_ERROR of type: " << type << "\n" << infoLog << "\n -- --------------------------------------------------- -- " << std::endl;
        }
    }
}
#pragma once
#include<glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "gl/glew.h"
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <map>
#include<vector>

using std::string;
using std::vector;
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
class Shader
{
public:
    std::map<string, int> uniformLocations;

    unsigned int ID=0;
    void shader(const char* vertexPath, const char* fragmentPath);
    void use();
    GLint  getUniformLocation(const char* name);
    void   setUniform(const char* name, float x, float y, float z);
    void   setUniform(const char* name, const vec2& v);
    void   setUniform(const char* name, const vec3& v);
    void   setUniform(const char* name, const vec4& v);
    void   setUniform(const char* name, const mat4& m);
    void   setUniform(const char* name, const mat3& m);
    void   setUniform(const char* name, float val);
    void   setUniform(const char* name, int val);
    void   setUniform(const char* name, bool val);
    void   setUniform(const char* name, GLuint val);

    void unuse();
private:

    void checkCompileErrors(unsigned int shader, std::string type);
};

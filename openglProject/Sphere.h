#pragma once
#include<vector>
#include <GL\glew.h>
#include"shader.h"
#include"sky.h"
using namespace std;
extern vec3 lightPosition;

extern vec3 viewPosition;
extern vec3 lightColor;

extern vec3 Ka;//������
extern vec3 Kd;//�������
extern vec3 Ks;//���淴���

extern int shiness;//�߹�ϵ��
class Sphere
{
public:
	~Sphere(void);

	void initData(float r);
	void render();

	void setTexture(GLuint texture);

	void loadShader(const char* vsfileName, const char* fsfileName);
	//const char* vsfile=nullptr, *fsfile=nullptr;
	Shader prog;
	unsigned int vboHandle[3];
	unsigned int vaoHandle;

	mat4 projectionMat = glm::mat4(1.0f);
	mat4 viewMat = glm::mat4(1.0f);
	mat4 model = glm::mat4(1.0f);
private:
	float fR=0.0f;

	GLuint textureID=0;

	int iCount=0;//�����θ���
};


#pragma once
#include"model.h"
#include"sphere.h"
class ImportModel {
public:
	void initShader(const char* vsfile, const char* fsfile);
	void initModel(const char* path);
	void render();
	Shader ourShader;
	Model ourModel;
	glm::mat4 model = mat4(1.0f);
	glm::mat4 model1 = mat4(1.0f);
	glm::mat4 model2 = mat4(1.0f);
	glm::mat4 projection = mat4(1.0f);
	glm::mat4 view = mat4(1.0f);
};
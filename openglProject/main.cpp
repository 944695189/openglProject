#pragma once
#include "sky.h"
#include"sphere.h"
#include"importModel.h"
#include "noisePattern.h"
using namespace std;

#define numVAOs 1
#define numVBOs 4

noisePattern noise = noisePattern();
float radius = 0.5f;
double lastX = 0.0;
double lastY = 0.0;
bool mousePressed = false;
float zPos = 0.0;
Sphere sphere;
GLuint textureID;
vec3 cameraPosition = vec3(3.0f, -1.50f, 6.0f);//相机位置
vec3 at = vec3(.0f, 0.0f, 0.0f);
float headdir[] = { 0.0f,.0f,1.0f };
float rightdir[] = {1.0f,.0f,0.0f };
float step = 0.10f;
float beta = 180.0f;//与z轴正向夹角
//显示区域宽高
int width, height;
//相机高度(水上/水下)、相机视场角、显示区域宽高比例
float cameraHeight = 2.0f, cameraHorizontal = 15.0f, cameraVertial= -10.0f,aspect;

//水面平面高度、水底平面高度
float surfacePlaneHeight = 0.0f, floorPlaneHeight = -10.0f;
GLuint vao[numVAOs], vbo[numVBOs];
//天空盒纹理、3D噪声纹理
GLuint skyboxTexture, noiseTexture;
Shader renderProgramSky;
Shader renderProgramSurface;
Shader renderProgramFloor;
ImportModel importModel;
//本地投影矩阵、视图矩阵、模型矩阵、mv矩阵、mv逆矩阵
glm::mat4 pMat, vMat, mMat, mvMat, inverseMat;
//点光源位置x,y,z
float lightPositon[3];
//shader Uniform：全局环境光、灯源环境光 漫反射、高光、光照位置、
GLuint globalAmbientLocation, lighting, diffuseReflection, highlight, lightLocation;
//shader Uniform：材质环境光、材质漫反射、材质高光、材质光泽度、位于水面上/水面下
GLuint materialAmbientLight, materialDiffuseReflection, materialHightlight, materialGloss, isAboveSurface;
//点光源位置、当前光源位置、光源转换
glm::vec3 pointLight = glm::vec3(0.0f, 4.0f, -8.0f), currentLightPosition, lightConversion;
float globalAmbient[4] = { 0.7f, 0.7f, 0.7f, 1.0f };
float lightAmbient[4] = { 0.0f, 0.0f, 0.0f, 1.0f };
float lightDiffuse[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float lightSpecular[4] = { 1.0f, 1.0f, 1.0f, 1.0f };

//材质
float m_materialAmbient[4] = { 0.5f, 0.6f, 0.8f, 1.0f };
float m_materialDiffuse[4] = { 0.8f, 0.9f, 1.0f, 1.0f };
float m_materialHighlight[4] = { 1.0f, 1.0f, 1.0f, 1.0f };
float m_materialGloss = 100.0f;

//折射、反射ID;折射、反射Buffer
GLuint refractTextureId;
GLuint reflectTextureId;
GLuint refractFrameBuffer;
GLuint reflectFrameBuffer;

//3D纹理噪声动态变化调整
float depthLookup = 0.0f;
GLuint offsetLocation;
double previousTime;

//初始化噪声Texture
GLuint initNoiseTexture() {
	GLuint textureID;
	int noiseH = noise.getHeight(), noiseW = noise.getWidth(), noiseD = noise.getDeep();
	GLubyte* data = new GLubyte[noiseH * noiseW * noiseD * 4];
	noise.fillDataArray(data);
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_3D, textureID);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexStorage3D(GL_TEXTURE_3D, 1, GL_RGBA8, noiseW, noiseH, noiseD);
	glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, noiseW, noiseH, noiseD, GL_RGBA, GL_UNSIGNED_INT_8_8_8_8_REV, data);
	delete[] data;
	return textureID;
}
//初始化光照
void installLights(glm::mat4 vMatrix, GLuint renderingProgram) {
	lightConversion = glm::vec3(vMatrix * glm::vec4(currentLightPosition, 1.0));
	lightPositon[0] = lightConversion.x;
	lightPositon[1] = lightConversion.y;
	lightPositon[2] = lightConversion.z;

	globalAmbientLocation = glGetUniformLocation(renderingProgram, "globalAmbient");
	lighting = glGetUniformLocation(renderingProgram, "light.ambient");
	diffuseReflection = glGetUniformLocation(renderingProgram, "light.diffuse");
	highlight = glGetUniformLocation(renderingProgram, "light.specular");
	lightLocation = glGetUniformLocation(renderingProgram, "light.position");
	materialAmbientLight = glGetUniformLocation(renderingProgram, "material.ambient");
	materialDiffuseReflection = glGetUniformLocation(renderingProgram, "material.diffuse");
	materialHightlight = glGetUniformLocation(renderingProgram, "material.specular");
	materialGloss = glGetUniformLocation(renderingProgram, "material.shininess");

	glProgramUniform4fv(renderingProgram, globalAmbientLocation, 1, globalAmbient);
	glProgramUniform4fv(renderingProgram, lighting, 1, lightAmbient);
	glProgramUniform4fv(renderingProgram, diffuseReflection, 1, lightDiffuse);
	glProgramUniform4fv(renderingProgram, highlight, 1, lightSpecular);
	glProgramUniform3fv(renderingProgram, lightLocation, 1, lightPositon);
	glProgramUniform4fv(renderingProgram, materialAmbientLight, 1, m_materialAmbient);
	glProgramUniform4fv(renderingProgram, materialDiffuseReflection, 1, m_materialDiffuse);
	glProgramUniform4fv(renderingProgram, materialHightlight, 1, m_materialHighlight);
	glProgramUniform1f(renderingProgram, materialGloss, m_materialGloss);
}

void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		mousePressed = true;
		glfwGetCursorPos(window, &lastX, &lastY);
	}
	else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
	{
		mousePressed = false;
	}
}
void keyboard(GLFWwindow* window, int key, int scancode, int action, int mods) {
	switch (key) {
	case GLFW_KEY_W:
		for (int i = 0; i < 3; i++)
			cameraPosition[i] += step * headdir[i];
		break;
	case GLFW_KEY_S:
		for (int i = 0; i < 3; i++)
			cameraPosition[i] -= step * headdir[i];
		break;
	case GLFW_KEY_A:
		for (int i = 0; i < 3; i++)
			cameraPosition[i] += step * rightdir[i];
		break;
	case GLFW_KEY_D:
		for (int i = 0; i < 3; i++)
			cameraPosition[i] -= step * rightdir[i];
		break;

	case GLFW_KEY_Z:
		cameraPosition[1] += 0.30f;
		break;
	case GLFW_KEY_X:
		cameraPosition[1] -= 0.30f;
		break;
	case GLFW_KEY_1:
		lightPosition[0] += 0.1f;
		break;
	case GLFW_KEY_2:
		lightPosition[0] -= 0.1f;
		break;
	}
}
void mouseCallback(GLFWwindow* window, double xpos, double ypos)
{
	if (!mousePressed)
		return;
	if (lastX == 0.0 && lastY == 0.0) {
		lastX = xpos;
		lastY = ypos;
		return;
	}
	float xoffset = (float)(lastX - xpos);
	float yoffset = (float)(lastY - ypos);
	lastX = (float)xpos;
	lastY = (float)ypos;
	float sensitivity = 0.1f;
	xoffset *= sensitivity;
	yoffset *= sensitivity;
	cameraHorizontal += xoffset;
	cameraVertial += yoffset;
}
void initVertices() {
	float cubeVertexPositions[108] =
	{ -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f, 1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, 1.0f, -1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f, 1.0f,  1.0f, -1.0f,
		1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, 1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,  1.0f,  1.0f,
		-1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f, -1.0f,
		1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,  1.0f,
		-1.0f,  1.0f, -1.0f, 1.0f,  1.0f, -1.0f, 1.0f,  1.0f,  1.0f,
		1.0f,  1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f, -1.0f
	};
	float PLANE_POSITIONS[18] = {
			-120.0f, 0.0f, -240.0f,  -120.0f, 0.0f, 0.0f,  120.0f, 0.0f, -240.0f,
			120.0f, 0.0f, -240.0f,  -120.0f, 0.0f, 0.0f,  120.0f, 0.0f, 0.0f
	};
	float PLANE_TEXCOORDS[12] = {
		0.0f, 0.0f,  0.0f, 1.0f,  1.0f, 0.0f,
		1.0f, 0.0f,  0.0f, 1.0f,  1.0f, 1.0f
	};
	float PLANE_NORMALS[18] = {
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,
		0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f,  0.0f, 1.0f, 0.0f
	};
	glGenVertexArrays(1, vao);
	glBindVertexArray(vao[0]);
	glGenBuffers(numVBOs, vbo);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertexPositions), cubeVertexPositions, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_POSITIONS), PLANE_POSITIONS, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_TEXCOORDS), PLANE_TEXCOORDS, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(PLANE_NORMALS), PLANE_NORMALS, GL_STATIC_DRAW);
}

//创建反射和折射buffer
void createReflectRefractBuffers(GLFWwindow* window) {
	GLuint bufferId[1];
	glGenBuffers(1, bufferId);
	glfwGetFramebufferSize(window, &width, &height);
	//创建反射自定义帧缓冲区
	glGenFramebuffers(1, bufferId);
	reflectFrameBuffer = bufferId[0];
	glBindFramebuffer(GL_FRAMEBUFFER, reflectFrameBuffer);
	glGenTextures(1, bufferId);
	reflectTextureId = bufferId[0];
	glBindTexture(GL_TEXTURE_2D, reflectTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//创建颜色纹理并将其绑定到自定义缓冲区中
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, reflectTextureId, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);

	//创建折射自定义帧缓冲区
	glGenFramebuffers(1, bufferId);
	refractFrameBuffer = bufferId[0];
	glBindFramebuffer(GL_FRAMEBUFFER, refractFrameBuffer);
	glGenTextures(1, bufferId);
	refractTextureId = bufferId[0];
	glBindTexture(GL_TEXTURE_2D, refractTextureId);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	//创建颜色纹理并将其绑定到自定义缓冲区中
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, refractTextureId, 0);
	glDrawBuffer(GL_COLOR_ATTACHMENT0);
}
void init(GLFWwindow* window) {
	renderProgramSky.shader("shader/vShaderCube.glsl", "shader/fShaderCube.glsl");
	renderProgramSurface.shader("shader/vShaderSurface.glsl", "shader/fShaderSurface.glsl");
	renderProgramFloor.shader("shader/vShaderFloor.glsl", "shader/fShaderFloor.glsl");
	//renderingProgramCubeMap = Utils::createShaderProgram("shader/vShaderCube.glsl", "shader/fShaderCube.glsl");
	initVertices();
	pointLight = glm::vec3(0.0f, 5.0f, -30.0f);
	skyboxTexture = sky::loadCubeMap("asset/cubeMap");
	sphere.initData(radius);
	sphere.loadShader("shader/vsplight.glsl", "shader/fsplight.glsl");
	sphere.setTexture(sky::loadTexture("asset/Textures/1662781130306.jpeg"));
	importModel.initShader("shader/vsplight.glsl", "shader/fsplight.glsl");
	importModel.initModel("asset/AmberDoll/Doll.pmx");
	glEnable(GL_TEXTURE_CUBE_MAP_SEAMLESS);
	createReflectRefractBuffers(window);
	noise.generateNoise();
	noiseTexture = initNoiseTexture();
	previousTime = glfwGetTime();
}
//绘制天空盒模型数据更新
void forSkyBoxRender() {
	renderProgramSky.use();
	renderProgramSky.setUniform("v_matrix", vMat);
	renderProgramSky.setUniform("p_matrix", pMat);
	isAboveSurface = renderProgramSky.getUniformLocation("isAbove");
	if (cameraHeight >= surfacePlaneHeight)
		glUniform1i(isAboveSurface, 1);
	else
		glUniform1i(isAboveSurface, 0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_CUBE_MAP, skyboxTexture);
}
//绘制水面模型数据及texture更新
void forTopSurfaceRender() {
	renderProgramSurface.use();
	renderProgramSurface.setUniform("mv_matrix", vMat);
	renderProgramSurface.setUniform("proj_matrix", pMat);
	renderProgramSurface.setUniform("norm_matrix", inverseMat);
	isAboveSurface = renderProgramSurface.getUniformLocation("isAbove");	
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, surfacePlaneHeight, 0.0f));
	mvMat = vMat * mMat;
	inverseMat = glm::transpose(glm::inverse(mvMat));
	currentLightPosition = glm::vec3(pointLight.x, pointLight.y, pointLight.z);
	installLights(vMat, renderProgramSurface.ID);

	if (cameraHeight >= surfacePlaneHeight)
		glUniform1i(isAboveSurface, 1);
	else
		glUniform1i(isAboveSurface, 0);

	offsetLocation = glGetUniformLocation(renderProgramSurface.ID, "depthOffset");
	glUniform1f(offsetLocation, depthLookup);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, reflectTextureId);
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, refractTextureId);
	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_3D, noiseTexture);
}

//绘制水下模型数据及texture更新
void forFloorRender() {
	renderProgramFloor.use();
	renderProgramFloor.setUniform("mv_matrix", vMat);
	renderProgramFloor.setUniform("proj_matrix", pMat);
	renderProgramFloor.setUniform("norm_matrix", inverseMat);

	isAboveSurface = renderProgramFloor.getUniformLocation("isAbove");
	mMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, floorPlaneHeight, 0.0f));
	mvMat = vMat * mMat;
	inverseMat = glm::transpose(glm::inverse(mvMat));

	currentLightPosition = glm::vec3(pointLight.x, pointLight.y, pointLight.z);
	installLights(vMat, renderProgramFloor.ID);

	if (cameraHeight >= surfacePlaneHeight)
		glUniform1i(isAboveSurface, 1);
	else
		glUniform1i(isAboveSurface, 0);

	offsetLocation = glGetUniformLocation(renderProgramFloor.ID, "depthOffset");
	glUniform1f(offsetLocation, depthLookup);

	glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[2]);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, vbo[3]);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(2);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, noiseTexture);
}

void display(GLFWwindow* window, double currentTime) {
	glfwGetFramebufferSize(window, &width, &height);
	aspect = (float)width / (float)height;
	pMat = glm::perspective(static_cast<float>DEG2RAD(60.0f), aspect, 0.1f, 1000.0f);
	//glm::perspective用于创建投影矩阵,用于将三维场景投影到二维视图上

	depthLookup += (currentTime - previousTime) * 0.05f;//动态流动效果
	previousTime = currentTime;
	if (cameraHeight > surfacePlaneHeight) {
		//相机观察矩阵，将相机沿着y轴上移，同时延x轴旋转cameraPitch角度,物体与相机做相反的变换
		vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -(surfacePlaneHeight - cameraHeight), 0.0f))
			* glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(cameraHorizontal), glm::vec3(.0f, 1.0f, .0f))
			* glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(-cameraVertial), glm::vec3(1.0f, 0.0f, .0f));

		glBindFramebuffer(GL_FRAMEBUFFER, reflectFrameBuffer);
		glClear(GL_DEPTH_BUFFER_BIT| GL_COLOR_BUFFER_BIT);
		forSkyBoxRender();
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 36); 
		glEnable(GL_DEPTH_TEST);
	}
	vMat = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, -(-(surfacePlaneHeight - cameraHeight)), zPos))
		* glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(cameraHorizontal), glm::vec3(.0f, 1.0f, .0f))
		* glm::rotate(glm::mat4(1.0f), static_cast<float>DEG2RAD(cameraVertial), glm::vec3(1.0f, 0.0f, .0f));
	//启用折射buffer
	glBindFramebuffer(GL_FRAMEBUFFER, refractFrameBuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if (cameraHeight >= surfacePlaneHeight) {
		forFloorRender();
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		glDrawArrays(GL_TRIANGLES, 0, 6);
	}
	else {
		forSkyBoxRender();
		glEnable(GL_CULL_FACE);
		glFrontFace(GL_CCW);
		glDisable(GL_DEPTH_TEST);
		glDrawArrays(GL_TRIANGLES, 0, 36);
		glEnable(GL_DEPTH_TEST);
	}
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glClear(GL_COLOR_BUFFER_BIT);

	forSkyBoxRender();

	glEnable(GL_CULL_FACE);
	glFrontFace(GL_CCW);
	glDisable(GL_DEPTH_TEST);
	glDrawArrays(GL_TRIANGLES, 0, 36);

	glEnable(GL_DEPTH_TEST);
	forTopSurfaceRender();
	glDepthFunc(GL_LEQUAL);
	if (cameraHeight >= surfacePlaneHeight)
		glFrontFace(GL_CCW);
	else
		glFrontFace(GL_CW);
	glDrawArrays(GL_TRIANGLES, 0, 6);
	forFloorRender();
	for (int i = 0; i < 3; i++)
	{
		at[i] = cameraPosition[i] - headdir[i];
	}
	sphere.projectionMat = pMat;	
	sphere.viewMat = glm::lookAt(cameraPosition, at, glm::vec3(.0f, 1.0f, .0f));
	sphere.render();
	
	importModel.render();
	importModel.view = sphere.viewMat;
	importModel.projection = pMat;
}
int main(void) {
	if (!glfwInit()) { exit(EXIT_FAILURE); }	
	GLFWwindow* window = glfwCreateWindow(1300,800, "Sky", NULL, NULL);
	glfwMakeContextCurrent(window);
	if (glewInit() != GLEW_OK) { exit(EXIT_FAILURE); }
	glfwSwapInterval(1);
	
	init(window);
	
	glfwSetMouseButtonCallback(window, mouseButtonCallback);
	
	glfwSetCursorPosCallback(window, mouseCallback);
	glfwSetKeyCallback(window, keyboard);
	while (!glfwWindowShouldClose(window)) {
		
		display(window, glfwGetTime());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwDestroyWindow(window);
	glfwTerminate();
	exit(1);
}

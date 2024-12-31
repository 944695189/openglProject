#pragma once
#include <glfw/glfw3.h>
#include <cmath>

#define PI 3.1415926535

const int noiseWidth = 256;
const int noiseHeight = 256;
const int noiseDepth = 256;

class noisePattern
{
private:
	int width{ noiseWidth };
	int height{ noiseHeight };
	int deep{ noiseDepth };
	double noiseMat[noiseHeight][noiseWidth][noiseDepth];
private:
	double smoothNoise(double zoom, double x1, double y1, double z1);
	double turbulence(double x, double y, double z, double maxZoom);

public:
	int getWidth() { return width; };
	int getHeight() { return height; };
	int getDeep() { return deep; };
	void fillDataArray(GLubyte data[]);
	void generateNoise();
};


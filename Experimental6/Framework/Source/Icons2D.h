#pragma once

#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>

class Icons2D
{
public:
	static void setCurrentShader(unsigned int shaderIDToSet);
	static void restoreOldShader();
	static void initializeIcon(const char * texturePath1, const char * texturePath2, const char * texturePath3, const char * texturePath4);
	static void displayIcon(int element, int x, int y, int size);
	static void cleanUpIcon();
	static void initStruct();
};

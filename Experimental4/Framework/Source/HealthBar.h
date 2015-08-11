#pragma once

#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>

class HealthBar
{
public:
	
	static void setCurrentShader();
	static void restoreOldShader();
	static void initializeHP(const char * texturePath);
	static void displayHP(int x, int y, int size);
	static void cleanUpHP();
	static void removeHP(int amountToRemove);
	static void addHP(int amountToAdd);
	static unsigned int GetRemainingHP();
};

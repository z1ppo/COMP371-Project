#pragma once

#include <GL/glew.h>

#include <vector>
#include <GLM/glm.hpp>

class Text2D
{
public:
	static void init2DText(const char * texturePath);
	static void print2DText(const char * text, int x, int y, int size);
	static void clean2DText();
};

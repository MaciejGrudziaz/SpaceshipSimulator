#pragma once
#include <GLFW/glfw3.h>

struct BlendFunctions
{
	GLenum sfactor;
	GLenum dfactor;

	BlendFunctions()
		: sfactor(GL_SRC_ALPHA)
		, dfactor(GL_ONE_MINUS_SRC_ALPHA)
	{}
};

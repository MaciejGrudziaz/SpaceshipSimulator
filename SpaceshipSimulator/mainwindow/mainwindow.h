#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>
#include <GameRenderer/renderobject.h>
#include <functional>
#include <iostream>

#include "../utilities/time.h"

class MainWindow
{
public:
	MainWindow(const std::string& title, int width = 1280, int height = 720, float fov = 45.0f);

	void create();
	void destory();

	glm::mat4 getProjection()const;
	std::shared_ptr<const glm::mat4> getProjectionPtr()const;

	void resize(int width, int height);
	int getWidth()const;
	int getHeight()const;

	void setFov(int fov);

	GLFWwindow* getGLFWwindow() const;

private:
	GLFWwindow* window;
	std::string title;
	int width, height;
	float fov;

	std::shared_ptr<glm::mat4> projection;

	void updateProjection();

	struct Resize
	{
		static MainWindow* currMainWindow;
		static void call(GLFWwindow* wnd, int width, int height)
		{
			if (currMainWindow)
				currMainWindow->resize(width, height);
		}
	};
};

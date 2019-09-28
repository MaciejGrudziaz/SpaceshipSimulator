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
	MainWindow(const std::string& title, int width = 1280, int height = 720, float fov = 90.0f);

	void create();
	void refresh();
	void destory();

	void addRenderer(RenderObjectPtr renderer);

	glm::mat4 getProjection()const;
	std::shared_ptr<const glm::mat4> getProjectionPtr()const;

	void resize(int width, int height);
	int getWidth()const;
	int getHeight()const;

	GLFWwindow* getGLFWwindow() const;

private:
	GLFWwindow* window;
	std::string title;
	int width, height;
	float fov;

	std::function<void(GLFWwindow*, int, int)> fun;

	std::shared_ptr<glm::mat4> projection;

	std::list<RenderObjectPtr> renderers;

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

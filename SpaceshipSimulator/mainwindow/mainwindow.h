#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <string>

class MainWindow
{
public:
	MainWindow(const std::string& title, int width = 1280, int height = 720, float fov = 90.0f);

	void create();
	void refresh();
	void destory();

private:
	int width, height;
	float fov;

	std::shared_ptr<glm::mat4> projection;

	void updateProjection();

};

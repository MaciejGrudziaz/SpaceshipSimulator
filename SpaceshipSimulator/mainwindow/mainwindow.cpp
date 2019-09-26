#include "mainwindow.h"

MainWindow::MainWindow(const std::string& title, int width = 1280, int height = 720, float fov = 90.0f)
	: width(width)
	, height(height)
	, fov(fov)
{
	updateProjection();
}

void MainWindow::updateProjection()
{
	*projection = glm::perspective(fov, static_cast<float>(width) / static_cast<float>(height), 0.01f, 1000.0f);
}

void MainWindow::create()
{
	GLFWwindow* window;

	if (!glfwInit())
		return -1;

	window = glfwCreateWindow(640, 480, "Hello world", NULL, NULL);
	if (!window)
	{
		glfwTerminate();
		return -1;
	}

	glfwMakeContextCurrent(window);

	glewInit();
}

void MainWindow::refresh()
{

}

void MainWindow::destory()
{

}

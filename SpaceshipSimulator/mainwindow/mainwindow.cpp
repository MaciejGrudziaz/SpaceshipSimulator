#include "mainwindow.h"

MainWindow* MainWindow::Resize::currMainWindow = nullptr;

MainWindow::MainWindow(const std::string& title, int width, int height, float fov)
	: title(title)
	, width(width)
	, height(height)
	, fov(fov)
	, projection(std::make_shared<glm::mat4>(1.0f))
	, window(nullptr)
{
	updateProjection();
}

void MainWindow::updateProjection()
{
	*projection = glm::perspective(fov, static_cast<float>(width) / static_cast<float>(height), 0.01f, 1000.0f);
}

void globalResize(GLFWwindow* wnd, int width, int height)
{
	std::cout << "global resize" << std::endl;
}

void MainWindow::create()
{
	if (glfwInit())
	{
		window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
		if (!window)
		{
			glfwTerminate();
		}

		if (!Resize::currMainWindow)
		{
			Resize::currMainWindow = this;
			glfwSetWindowSizeCallback(window, &Resize::call);
		}

		glfwMakeContextCurrent(window);

		glewInit();
	}
}

void MainWindow::destory()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

glm::mat4 MainWindow::getProjection()const
{
	return *projection;
}

std::shared_ptr<const glm::mat4> MainWindow::getProjectionPtr()const
{
	return projection;
}

void MainWindow::resize(int width, int height)
{
	this->width = width;
	this->height = height;

	updateProjection();
}

int MainWindow::getWidth()const
{
	return width;
}

int MainWindow::getHeight()const
{
	return height;
}

void MainWindow::setFov(int fov)
{
	if (!window)
		this->fov = fov;

	updateProjection();
}

GLFWwindow* MainWindow::getGLFWwindow() const
{
	return window;
}
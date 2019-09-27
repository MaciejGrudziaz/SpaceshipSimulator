#include "mainwindow.h"

MainWindow* MainWindow::Resize::currMainWindow = nullptr;

MainWindow::MainWindow(const std::string& title, int width, int height, float fov)
	: title(title)
	, width(width)
	, height(height)
	, fov(fov)
	, projection(std::make_shared<glm::mat4>(1.0f))
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
		glfwWindowHint(GLFW_SAMPLES, 16);

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

void MainWindow::refresh()
{
	while (!glfwWindowShouldClose(window))
	{
		glViewport(0, 0, width, height);
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		std::for_each(renderers.begin(), renderers.end(), [](auto renderer) { 
			renderer->process(); 
		});

		glfwSwapBuffers(window);
		glfwPollEvents();
	}
}

void MainWindow::destory()
{
	glfwDestroyWindow(window);
	glfwTerminate();
}

void MainWindow::addRenderer(RenderObjectPtr renderer)
{
	renderers.push_back(renderer);
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
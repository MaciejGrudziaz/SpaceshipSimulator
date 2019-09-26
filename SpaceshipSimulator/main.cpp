#define GLEW_STATIC
#include <gl/glew.h>
#include <iostream>
#include <GLFW/glfw3.h>

#include <GameResources/files/mgrimportfile.h>

#define DEBUG

#ifdef DEBUG
int main(void)
#else
int WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR     lpCmdLine,
	int       nShowCmd
)
#endif
{
	MgrImportFile mgrImport;
	mgrImport.import("models/spaceship.mgr");

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

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);
		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	glfwTerminate();
	return 0;
}
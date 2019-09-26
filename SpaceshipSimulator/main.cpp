#include <GameResources/gameobjects/camera.h>
#include "mainwindow/mainwindow.h"
#include "gameobjects/spaceship.h"

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
	Camera camera;
	camera.getTransform().setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	camera.update();

	Spaceship spaceship;
	spaceship.load("models/spaceship_tex.mgr");

	MainWindow wnd("Main Window");

	wnd.create();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	//spaceship.loadShaders("shaders/spaceshipShader.vert", "shaders/spaceshipShader.frag");
	//spaceship.initRenderer(camera.getViewPtr(), wnd.getProjectionPtr());
	//spaceship.loadBufferData();

	//wnd.addRenderer(spaceship.getRenderer());

	spaceship.loadTextureShaders("shaders/spaceshipShaderTex.vert", "shaders/spaceshipShaderTex.frag");
	spaceship.initTextureRenderer(camera.getViewPtr(), wnd.getProjectionPtr());
	spaceship.loadTextureBufferData();
	spaceship.loadTexture("models/textures/texture.png");

	wnd.addRenderer(spaceship.getTextureRenderer());

	wnd.refresh();
	wnd.destory();

	return 0;
}
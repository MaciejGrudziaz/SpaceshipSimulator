#include <conio.h>
#include <GameResources/gameobjects/camera.h>
#include "mainwindow/mainwindow.h"
#include "gameobjects/spaceship.h"
#include "gameengine.h"

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
	GameEngine engine;
	
	engine.launch();
	engine.process();
	engine.end();

	//Camera camera;
	//camera.getTransform().setPosition(glm::vec3(0.0f, 0.0f, 3.0f));
	//camera.update();

	//MainWindow wnd("Main Window");

	//wnd.create();

	//glEnable(GL_MULTISAMPLE);
	//glEnable(GL_DEPTH_TEST);

	//SpaceshipData filesData;
	//filesData.modelFilename = "models/spaceship_tex.mgr";
	//filesData.vertexShaderFilename = "shaders/spaceshipShader.vert";
	//filesData.fragmentShaderFilename = "shaders/spaceshipShader.frag";
	//filesData.textureFilename = "models/textures/texture.png";

	//SpaceshipUniforms uniforms;
	//uniforms.view = camera.getViewPtr();
	//uniforms.projection = wnd.getProjectionPtr();

	//Spaceship spaceship;
	//spaceship.load(filesData, uniforms);

	//wnd.addRenderer(spaceship.getRenderer());

	//wnd.refresh();
	//wnd.destory();

	_getch();
	return 0;
}
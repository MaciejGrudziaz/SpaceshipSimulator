#include <GameResources/gameobjects/camera.h>
#include "mainwindow/mainwindow.h"
#include "gameobjects/spaceship.h"
#include "gameengine.h"
#include <Windows.h>

#define RELEASE

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

	return 0;
}
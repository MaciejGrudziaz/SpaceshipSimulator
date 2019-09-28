#include "gamemain.h"

GAME_INIT(engine)
{
	setCamera(engine);
	loadSpaceship(engine);
	registerInput(engine);

	engine.getResources()->spaceship->init();
}

GAME_PROCESS(engine)
{
	//updateCamera(engine);
	engine.getResources()->camera->update();
	engine.getResources()->spaceship->process();
}

GAME_END(engine)
{

}

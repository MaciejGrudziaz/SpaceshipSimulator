#include "gamemain.h"

GAME_INIT(engine)
{
	srand(time(0));

	setCamera(engine);
	loadSpaceship(engine);
	loadAsteroids(engine);
	loadTestAsteroid(engine);
	registerInput(engine);
	registerAsteroidBehaviours(engine);

	engine.getResources()->spaceship->init();
	
	std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
		asteroid->init();
	});

	engine.getResources()->testAsteroid->init();
}

GAME_PROCESS(engine)
{
	engine.getResources()->camera->update();
	engine.getResources()->spaceship->process();
	
	std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
		asteroid->process();
	});

	engine.getResources()->testAsteroid->process();
}

GAME_END(engine)
{

}

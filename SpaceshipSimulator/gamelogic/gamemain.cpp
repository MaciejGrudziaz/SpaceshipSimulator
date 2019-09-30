#include "gamemain.h"

GAME_INIT(engine)
{
	srand(time(0));

	setCamera(engine);
	loadSpaceship(engine);
	loadPatternAsteroid(engine);
	loadAsteroids(engine);

	registerInput(engine);
	loadObjectsCollisionProperty(engine);

	registerAsteroidBehaviours(engine);

	engine.getResources()->spaceship->init();
	
	//engine.getResources()->asteroidPattern->init();

	std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
		asteroid->init();
	});
}

GAME_PROCESS(engine)
{
	engine.getResources()->camera->update();
	engine.getResources()->spaceship->process();
	
	std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
		asteroid->process();
	});

	//engine.getResources()->asteroidPattern->process();
}

GAME_END(engine)
{

}

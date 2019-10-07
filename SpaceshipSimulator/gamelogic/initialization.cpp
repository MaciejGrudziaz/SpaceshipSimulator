#include "initialization.h"

void setCamera(GameEngine& engine)
{
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->getTransform().setPosition(glm::vec3(0.0f, 10.0f, 40.0f));

	camera->update();
	engine.setCamera(camera);
	engine.getResources()->camera = camera;
	engine.getGui()->registerCamera(camera);
}

void loadWorldSpeed(GameEngine& engine)
{
	GameConfigData config = engine.getConfigData();
	engine.getResources()->worldSpeed = std::make_shared<float>();
	*(engine.getResources()->worldSpeed) = config.forwardSpeed;
}

//---------------------------------------------------------------------------------------
//---------------------------------------MAIN GAME---------------------------------------
//---------------------------------------------------------------------------------------

void initializeGame(GameEngine& engine)
{
	loadWorldSpeed(engine);
	engine.getResources()->collisionsManager = std::make_shared<CollisionsManager>();

	setCamera(engine);
	loadSpaceship(engine);
	loadAsteroids(engine);

	registerInput(engine);
	registerSpaceshipGunBehaviours(engine);
	registerSpaceshipEnginesControl(engine);
	registerSpaceshipCollisionSystem(engine);

	engine.getResources()->spaceship->init();

	engine.getResources()->asteroids->init();

	engine.getResources()->collisionsManager->run();
}

void processGame(GameEngine& engine)
{
	engine.getResources()->camera->update();
	engine.getResources()->spaceship->process();
	
	engine.getResources()->asteroids->process();

	engine.getResources()->collisionsManager->process();
}

void restartGame(GameEngine& engine)
{
	engine.registerPointScore(engine.getResources()->spaceship->getPointsScore());

	engine.getResources()->asteroids->restart();
	processGame(engine);
	engine.getResources()->spaceship->restart();
}

void invalidateGame(GameEngine& engine)
{
	engine.getResources()->spaceship->invalidate();

	engine.getResources()->asteroids->invalidate();

	engine.getResources()->collisionsManager->invalidate();
}

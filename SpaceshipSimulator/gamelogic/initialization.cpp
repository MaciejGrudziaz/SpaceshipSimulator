#include "initialization.h"

void setCamera(GameEngine& engine)
{
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->getTransform().setPosition(glm::vec3(0.0f, 12.0f, 20.0f));

	camera->update();
	engine.setCamera(camera);
	engine.getResources()->camera = camera;
}

//---------------------------------------------------------------------------------------
//---------------------------------------MAIN GAME---------------------------------------
//---------------------------------------------------------------------------------------

void initializeGame(GameEngine& engine)
{
	srand(time(0));

	engine.getResources()->worldSpeed = std::make_shared<float>(0.0f);
	engine.getResources()->collisionsManager = std::make_shared<CollisionsManager>();

	setCamera(engine);
	loadSpaceship(engine);
	//loadPatternAsteroid(engine);
	loadAsteroids(engine);

	registerInput(engine);
	loadObjectsCollisionProperty(engine);
	registerAsteroidBehaviours(engine);
	registerSpaceshipGunBehaviours(engine);
	registerSpaceshipEnginesControl(engine);

	engine.getResources()->spaceship->init();

	engine.getResources()->asteroids->init();
	//std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
	//	asteroid->init();
	//});
}

void processGame(GameEngine& engine)
{
	engine.getResources()->camera->update();
	engine.getResources()->spaceship->process();
	
	engine.getResources()->asteroids->process();

	engine.getResources()->collisionsManager->process();
	//std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
	//	asteroid->process();
	//});
}

void invalidateGame(GameEngine& engine)
{
	engine.getResources()->spaceship->invalidate();

	engine.getResources()->asteroids->invalidate();

	engine.getResources()->collisionsManager->invalidate();
	//std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
	//	asteroid->invalidate();
	//});
}

//---------------------------------------------------------------------------------------
//------------------------------------PARTICLES TEST-------------------------------------
//---------------------------------------------------------------------------------------

void initializeParticlesTest(GameEngine& engine)
{
	setCamera(engine);
	initializeParticleSystem(engine);
}

void processParticlesTest(GameEngine& engine)
{
	engine.getResources()->camera->update();
	processParticleSystem(engine);
}

void invalidateParticles(GameEngine& engine)
{
	engine.getResources()->particles->invalidate();
	engine.getResources()->particles2->invalidate();
}

void loadObjectsCollisionProperty(GameEngine& engine)
{
	//engine.getResources()->spaceship->addProperty<ObjectCollision>("collision");

	//engine.getResources()->asteroids->addAsteroidsProperty<ObjectCollision>("collision");

	engine.getResources()->collisionsManager->registerCollisionObject(engine.getResources()->spaceship);

	for (auto asteroid : engine.getResources()->asteroids->getAsteroids())
	{
		engine.getResources()->collisionsManager->registerCollisionObject(asteroid);
	}

	engine.getResources()->collisionsManager->run();
}
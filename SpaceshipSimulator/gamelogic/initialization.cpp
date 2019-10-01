#include "initialization.h"

void setCamera(GameEngine& engine)
{
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 30.0f));

	engine.setCamera(camera);
	engine.getResources()->camera = camera;
}

void initializeGame(GameEngine& engine)
{
	srand(time(0));

	setCamera(engine);
	loadSpaceship(engine);
	loadPatternAsteroid(engine);
	loadAsteroids(engine);

	registerInput(engine);
	loadObjectsCollisionProperty(engine);
	registerAsteroidBehaviours(engine);
	registerSpaceshipGunBehaviours(engine);

	engine.getResources()->spaceship->init();

	std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
		asteroid->init();
	});
}

void processGame(GameEngine& engine)
{
	engine.getResources()->camera->update();
	engine.getResources()->spaceship->process();

	std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid) {
		asteroid->process();
	});
}

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

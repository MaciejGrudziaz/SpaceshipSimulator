#include "asteroidinitialization.h"

void loadAsteroids(GameEngine& engine)
{
	AsteroidsManagerPtr asteroids = std::make_shared<AsteroidsManager>();
	engine.getResources()->asteroids = asteroids;
	loadAsteroidsConfigFileData(engine);

	asteroids->registerWorldSpeed(engine.getResources()->worldSpeed);
	asteroids->registerCollisionsManager(engine.getResources()->collisionsManager);
	asteroids->create(50, engine.getResources()->camera, engine.getProjectionMatPtr());
	
	engine.addRenderer(asteroids->getRenderer());
	engine.addRenderer(asteroids->getExplosionParticlesRenderer());
	engine.addRenderer(asteroids->getExplosionFragmentsParticlesRenderer());
	engine.getResources()->asteroids->setShowHitboxFlag(engine.getShowHitboxesFlag());
}

void loadAsteroidsConfigFileData(GameEngine& engine)
{
	GameConfigData config = engine.getConfigData();
	AsteroidsManagerPtr asteroids = engine.getResources()->asteroids;
	
	asteroids->setAsteroidDefualtFrequencySpawn(config.asteroidsSpawnFreq);
	asteroids->setAsteroidSpawnRateAcceleration(config.asteroidsSpawnAcceleration);
	asteroids->setAsteroidStandardHealth(config.asteroidStandardHealth);
	asteroids->setAsteroidMinAngularRot(config.asteroidMinAngualrVelocity);
	asteroids->setAsteroidMaxAngularRot(config.asteroidMaxAngularVelocity);
	asteroids->setExplosionParticlesLifetime(config.explosionDuration);
	asteroids->setParticlesCountFactor(config.particlesCountFactor);
	asteroids->setShowHitboxFlag(config.showHitboxes);
}

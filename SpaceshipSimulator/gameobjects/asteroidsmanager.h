#pragma once

#include <random>
#include "../renderers/multipleasteroidsrenderer.h"
#include "multisourceparticlesystem.h"
#include "asteroid.h"
#include "hitboxobject.h"
#include "particlesystem.h"

struct SpawnLimits
{
	float minX, maxX, minY, maxY;

	SpawnLimits()
		: minX(0.0f)
		, maxX(0.0f)
		, minY(0.0f)
		, maxY(0.0f)
	{}
};

class AsteroidsManager: public GameObject
{
public:
	AsteroidsManager();

	void create(int initialCount, CameraPtr camera, ConstMat4Ptr projectionPtr);

	void init()override;
	void process()override;
	void invalidate()override;

	void registerWorldSpeed(std::shared_ptr<float> speed);

	template<class T>
	void addAsteroidsProperty(const std::string& name);

	std::vector<AsteroidPtr>& getAsteroids();

	RenderObjectPtr getRenderer()const;
	//MultiSourceParticleRendererPtr getParticlesRenderer()const;

private:
	MultipleAsteroidsRendererPtr renderer;
	CameraPtr camera;
	ConstMat4Ptr projectionPtr;

	//MultiSourceParticleSystemPtr particlesSystem;

	AsteroidPtr patternAsteroid;
	HitboxObjectPtr asteroidHitboxPattern;
	ParticleSystemPtr asteroidsExplosionFragmentsPattern;
	ParticleSystemPtr asteroidsExplosionPattern;

	std::vector<AsteroidPtr> asteroids;
	SpawnLimits arenaLimits;

	std::shared_ptr<float> worldSpeed;

	std::mt19937 rng;

	void loadParticlesSystemRenderer();

	void createPatternAsteroid();
	void createPatternAsteroidHitbox();
	void createPatternAsteroidExplosionParticleSystem();

	void initializeAsteroidsVector(int initialCount);
	void initializeAsteroid(AsteroidPtr asteroid);

	void findLimitPosX();
	void findLimitPosY();
};

template<class T>
void AsteroidsManager::addAsteroidsProperty(const std::string& name)
{
	for (auto asteroid : asteroids)
	{
		asteroid->addProperty<T>(name);
	}
}

typedef std::shared_ptr<AsteroidsManager> AsteroidsManagerPtr;

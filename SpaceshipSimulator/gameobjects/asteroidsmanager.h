#pragma once

#include <random>
#include "../renderers/multipleasteroidsrenderer.h"
#include "../renderers/multisourceparticlerenderer.h"
#include "asteroid.h"
#include "hitboxobject.h"
#include "collisionsmanager.h"

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
	void registerCollisionsManager(CollisionsManagerPtr collisionsManager);

	template<class T>
	void addAsteroidsProperty(const std::string& name);

	std::vector<AsteroidPtr>& getAsteroids();

	RenderObjectPtr getRenderer()const;
	MultiSourceParticleRendererPtr getExplosionParticlesRenderer()const;
	MultiSourceParticleRendererPtr getExplosionFragmentsParticlesRenderer()const;

	void restart();

	void setAsteroidDefualtFrequencySpawn(float val) { asteroidDefualtFrequencySpawn = val; asteroidFrequencySpawn = asteroidDefualtFrequencySpawn; }
	void setAsteroidSpawnRateAcceleration(float val) { asteroidSpawnRateAcceleration = val; }
	void setAsteroidStandardHealth(float val) { asteroidStandardHealth = val; }
	void setAsteroidMinAngularRot(float val) { asteroidMinAngularRot = val; }
	void setAsteroidMaxAngularRot(float val) { asteroidMaxAngularRot = val; }
	void setParticlesCountFactor(float val) { particlesCountFactor = val; }
	void setExplosionParticlesLifetime(float val) { explosionParticlesLifetime = val; }
	void setShowHitboxFlag(bool val) { showHitboxFlag = val; }

private:
	CollisionsManagerPtr collisionsManager;

	MultipleAsteroidsRendererPtr renderer;
	CameraPtr camera;
	ConstMat4Ptr projectionPtr;

	ParticleSystemV2Ptr explosionParticles;
	ParticleSystemV2Ptr explosionFragmentsParticles;
	ParticleSystemDataPtr explosionParticlesUniformData;

	MultiSourceParticleRendererPtr explosionParticlesRenderer;
	MultiSourceParticleRendererPtr explosionFragmentsParticlesRenderer;
	ParticleSystemDataPtr explosionFragmentsParticlesUniformData;

	AsteroidPtr patternAsteroid;
	HitboxObjectPtr asteroidHitboxPattern;

	std::vector<AsteroidPtr> asteroids;
	SpawnLimits arenaLimits;

	std::shared_ptr<float> worldSpeed;

	std::mt19937 rng;

	float asteroidDefualtFrequencySpawn;
	float asteroidSpawnRateAcceleration;
	float asteroidStandardHealth;
	float asteroidMinAngularRot;
	float asteroidMaxAngularRot;
	float particlesCountFactor;
	float explosionParticlesLifetime;
	bool showHitboxFlag;

	float asteroidFrequencySpawn;
	float timeFromLastAsteroidSpawn;

	void loadParticlesSystemRenderer();

	void createPatternAsteroid();
	void createPatternAsteroidHitbox();

	void initializeAsteroidsVector(int initialCount);
	void createAsteroid();
	void initializeAsteroid(AsteroidPtr asteroid);

	void initializeParticlesSystem();
	void initializeExplosionParticleSystem();
	void initializeExplosionFragmentsParticleSystem();
	void initUniforms();
	void initExplosionParticlesUniform();
	void initExplosionFragmentParticlesUniform();

	void spawnAsteroids();
	bool activateAsteroid();

	//finding limit x and y position in global coordinates when the point is still visible on screen (clip space)
	void findLimitPosX();
	float findLimitPosXMinVal(float eps = 0.05f);
	float findLimitPosXMaxVal(float eps = 0.05f);
	void findLimitPosY();
	float findLimitPosYMinVal(float eps = 0.05f);
	float findLimitPosYMaxVal(float eps = 0.05f);
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

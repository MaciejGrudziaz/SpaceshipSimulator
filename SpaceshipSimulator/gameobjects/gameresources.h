#pragma once

#include "particlesystem.h"
#include "particlesystemv2.h"
#include "spaceship.h"
#include "../gameengine.h"
#include "../gameenginemacros.h"
#include "hitboxobject.h"
#include "asteroidsmanager.h"
#include "collisionsmanager.h"

GAME_RESOURCES
{
public:
	CameraPtr camera;

	SpaceshipPtr spaceship;

	StandardGameObjectPtr asteroidPattern;
	HitboxObjectPtr asteroidHitboxPattern;
	
	//std::vector<StandardGameObjectPtr> asteroids;
	AsteroidsManagerPtr asteroids;

	std::shared_ptr<float> worldSpeed;

	CollisionsManagerPtr collisionsManager;

	ParticleSystemPtr particles;
	ParticleSystemPtr particles2;

	GameResources()
		: camera(std::make_shared<Camera>())
		, spaceship(std::make_shared<Spaceship>())
	{}
};

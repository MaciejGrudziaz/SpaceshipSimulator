#pragma once

#include "spaceship.h"
#include "../gameengine.h"
#include "../gameenginemacros.h"
#include "hitboxobject.h"

GAME_RESOURCES
{
public:
	CameraPtr camera;

	SpaceshipPtr spaceship;

	//HitboxObjectPtr testHitbox;
	StandardGameObjectPtr asteroidPattern;
	HitboxObjectPtr asteroidHitboxPattern;
	
	//StandardGameObjectPtr testAsteroid;
	std::vector<StandardGameObjectPtr> asteroids;

	GameResources()
		: camera(std::make_shared<Camera>())
		, spaceship(std::make_shared<Spaceship>())
	{}
};

#pragma once

#include "spaceship.h"
#include "../gameengine.h"
#include "../gameenginemacros.h"

GAME_RESOURCES
{
public:
	CameraPtr camera;
	StandardGameObjectPtr spaceship;

	GameObjectPtr testAsteroid;
	std::vector<StandardGameObjectPtr> asteroids;

	GameResources()
		: camera(std::make_shared<Camera>())
		, spaceship(std::make_shared<Spaceship>())
	{}
};

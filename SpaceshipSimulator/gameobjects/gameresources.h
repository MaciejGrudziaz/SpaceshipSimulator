#pragma once

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
	
	AsteroidsManagerPtr asteroids;

	CollisionsManagerPtr collisionsManager;

	std::shared_ptr<float> worldSpeed;

	GameResources()
		: camera(std::make_shared<Camera>())
		, spaceship(std::make_shared<Spaceship>())
	{}

	void restart() {}
};

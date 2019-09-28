#pragma once

#include "../gameengine.h"
#include "../gameenginemacros.h"

GAME_RESOURCES
{
public:
	CameraPtr camera;
	SpaceshipPtr spaceship;

	GameResources()
		: camera(std::make_shared<Camera>())
		, spaceship(std::make_shared<Spaceship>())
	{}
};

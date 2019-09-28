#pragma once

#include "../gameengine.h"

template<>
class GameResources<GameEngine> {
public:
	CameraPtr camera;
	SpaceshipPtr spaceship;

	GameResources()
		: camera(std::make_shared<Camera>())
		, spaceship(std::make_shared<Spaceship>())
	{}
};

typedef std::shared_ptr<GameResources<GameEngine> > GameResourcesPtr;
#pragma once

#include "../dev/particles.h"
#include "spaceship.h"
#include "../gameengine.h"
#include "../gameenginemacros.h"
#include "hitboxobject.h"

GAME_RESOURCES
{
public:
	CameraPtr camera;

	SpaceshipPtr spaceship;

	StandardGameObjectPtr asteroidPattern;
	HitboxObjectPtr asteroidHitboxPattern;
	
	std::vector<StandardGameObjectPtr> asteroids;

	ParticleSystemPtr particles;

	GameResources()
		: camera(std::make_shared<Camera>())
		, spaceship(std::make_shared<Spaceship>())
	{}
};

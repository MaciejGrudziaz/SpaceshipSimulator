#pragma once

#include "standardgameobject.h"

class Spaceship: public StandardGameObject
{
public:
	Spaceship();

};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;

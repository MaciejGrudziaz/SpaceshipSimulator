#pragma once

#include "standardgameobject.h"

class Asteroid: public StandardGameObject
{
public:
	Asteroid();

};

typedef std::shared_ptr<Asteroid> AsteroidPtr;

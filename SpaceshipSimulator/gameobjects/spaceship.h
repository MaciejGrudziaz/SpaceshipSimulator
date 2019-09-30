#pragma once

#include "standardgameobject.h"
#include "spaceshipgun.h"

class Spaceship: public StandardGameObject
{
public:
	Spaceship();

	void loadGuns();

private:
	void loadStandardBufferData()override;
	void loadTextureBufferData()override;
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;

#pragma once

#include "standardgameobject.h"

struct SpaceshipData
{
	std::shared_ptr<BasicObject> model;
	int jointIdx;
	std::shared_ptr<RenderObject> renderer;
	bool textureAvailable;
};

class SpaceshipGun : public StandardGameObject
{
public:
	void load(const SpaceshipData& data);

private:
	void initBuffer();
	void loadStandardBufferData(const SpaceshipData& data);
	void loadTextureBufferData(const SpaceshipData& data);
};

typedef std::shared_ptr<SpaceshipGun> SpaceshipGunPtr;

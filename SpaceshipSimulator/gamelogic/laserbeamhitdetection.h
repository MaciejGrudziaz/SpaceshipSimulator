#pragma once

#include "../gameobjects/spaceship.h"
#include "../gameobjects/laserbeam.h"
#include "collisiondata.h"
#include <mutex>

class LaserBeamHitDetection : public Property<GameObject>
{
public:
	LaserBeamHitDetection(const std::string& name, GameObject& object);

	void init()override;
	void process()override;
	void invalidate()override;

	void addLaserBeam(LaserBeamPtr laserBeam);
	std::vector<LaserBeamPtr>& getLaserBeamsVec();

	void addCollisionData(CollisionDataPtr data);

	void initCleanLaserBeamsBuffer();
	void initCleanCollisionDataBuffer();

private:
	std::vector<LaserBeamPtr> laserBeams;
	std::vector<CollisionDataPtr> collisionData;

	int findNotActiveLaserIdx();
	int findNotActiveCollisionDataIdx();
};


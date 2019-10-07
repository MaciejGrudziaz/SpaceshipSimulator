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
	void releaseLaserBeamVec();

	void addCollisionData(const CollisionData& data);

private:
	std::vector<LaserBeamPtr> laserBeams;
	std::list<CollisionData> collisionData;

	std::mutex colDataMutex;
	std::mutex laserBeamMutex;
	bool colPtVecAvailableFlag;

	int findNotActiveLaserIdx();
};


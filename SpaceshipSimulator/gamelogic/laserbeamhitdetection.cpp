#include "laserbeamhitdetection.h"

LaserBeamHitDetection::LaserBeamHitDetection(const std::string& name, GameObject& object)
	: Property<GameObject>(name, object)
	, colPtVecAvailableFlag(true)
{}

void LaserBeamHitDetection::init()
{

}

void LaserBeamHitDetection::process()
{
	if (colPtVecAvailableFlag)
	{
		colDataMutex.lock();

		for (auto colData : collisionData)
		{
				std::string colliderName = colData.externalCollisionObj.getName();
				size_t pos = colliderName.find_last_of('_');
				colliderName = colliderName.substr(pos + 1);

				if (colliderName != "spaceship")
				{
					colData.internalCollisionObj.setActive(false);
					Spaceship& spaceship = static_cast<Spaceship&>(object);
					spaceship.addScore(10);
				}
		}

		

		collisionData.clear();
		colDataMutex.unlock();
	}
}

void LaserBeamHitDetection::invalidate()
{}

void LaserBeamHitDetection::addLaserBeam(LaserBeamPtr laserBeam)
{
	colDataMutex.lock();

	int idx = findNotActiveLaserIdx();
	if (idx != -1) laserBeams[idx] = laserBeam;
	else laserBeams.push_back(laserBeam);

	colDataMutex.unlock();
}

int LaserBeamHitDetection::findNotActiveLaserIdx()
{
	int idx = -1;
	bool found = false;

	if (laserBeams.size() > 0)
	{
		for (int i = 0; i < laserBeams.size(); ++i)
		{
			if (!laserBeams[i]->isActive())
			{
				idx = i;
				found = true;
			}
		}
	}

	return idx;
}

std::vector<LaserBeamPtr>& LaserBeamHitDetection::getLaserBeamsVec()
{
	colDataMutex.lock();
	colPtVecAvailableFlag = false;
	colDataMutex.unlock();

	return laserBeams;
}

void LaserBeamHitDetection::releaseLaserBeamVec()
{
	colDataMutex.lock();
	colPtVecAvailableFlag = true;
	colDataMutex.unlock();
}

void LaserBeamHitDetection::addCollisionData(const CollisionData& data)
{
	colDataMutex.lock();
	collisionData.push_back(data);
	colDataMutex.unlock();
}


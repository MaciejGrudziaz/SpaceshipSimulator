#include "laserbeamhitdetection.h"

LaserBeamHitDetection::LaserBeamHitDetection(const std::string& name, GameObject& object)
	: Property<GameObject>(name, object)
{
	initCleanLaserBeamsBuffer();
	initCleanCollisionDataBuffer();
}

void LaserBeamHitDetection::initCleanLaserBeamsBuffer()
{
	laserBeams.clear();
	laserBeams = std::vector<LaserBeamPtr>(1000);
	for (int i = 0; i < laserBeams.size(); ++i)
	{
		laserBeams[i] = std::make_shared<LaserBeam>();
		laserBeams[i]->setActive(false);
	}
}

void LaserBeamHitDetection::initCleanCollisionDataBuffer()
{
	collisionData.clear();
	collisionData = std::vector<CollisionDataPtr>(1000);
	for (int i = 0; i < collisionData.size(); ++i)
	{
		collisionData[i] = std::make_shared<CollisionData>(object,object);
		collisionData[i]->isActive = false;
	}
}

void LaserBeamHitDetection::init()
{}

void LaserBeamHitDetection::process()
{
		for (auto colData : collisionData)
		{
			if (colData->isActive)
			{
				std::string colliderName = colData->externalCollisionObj.getName();
				size_t pos = colliderName.find_last_of('_');
				colliderName = colliderName.substr(pos + 1);

				if (colliderName != "spaceship")			
				{
					colData->internalCollisionObj.setActive(false);				//turning off laser beam
					Spaceship& spaceship = static_cast<Spaceship&>(object);		
					spaceship.addScore(10);
				}

				colData->isActive = false;
			}
		}
}

void LaserBeamHitDetection::invalidate()
{}

void LaserBeamHitDetection::addLaserBeam(LaserBeamPtr laserBeam)
{
	int idx = findNotActiveLaserIdx();
	if(idx!=-1) laserBeams[idx] = laserBeam;
}

int LaserBeamHitDetection::findNotActiveLaserIdx()
{
	static int idx = 0;
	bool found = false;

	for (int i = idx; i < laserBeams.size(); ++i)
	{
		if (!laserBeams[i]->isActive())
		{
			idx = i;
			found = true;
			break;
		}
	}

	if (!found)
	{
		for (int i = 0; i < idx; ++i)
		{
			if (!laserBeams[i]->isActive())
			{
				idx = i;
				found = true;
				break;
			}
		}
	}

	if (!found) return -1;

	return idx;
}

int LaserBeamHitDetection::findNotActiveCollisionDataIdx()
{
	static int idx = 0;
	bool found = false;

	for (int i = idx; i < collisionData.size(); ++i)
	{
		if (!(collisionData[i]->isActive))
		{
			idx = i;
			found = true;
			break;
		}
	}

	if (!found)
	{
		for (int i = 0; i < idx; ++i)
		{
			if (!(collisionData[i]->isActive))
			{
				idx = i;
				found = true;
				break;
			}
		}
	}

	if (!found) return -1;

	return idx;
}

std::vector<LaserBeamPtr>& LaserBeamHitDetection::getLaserBeamsVec()
{
	return laserBeams;
}

void LaserBeamHitDetection::addCollisionData(CollisionDataPtr data)
{
	int idx = findNotActiveCollisionDataIdx();
	if (idx != -1) collisionData[idx] = data;
}

#include "collisionsmanager.h"

CollisionsManager::CollisionsManager()
	: runFlag(false)
	, updateFlag(false)
	, warmupCounter(0)
{}

void CollisionsManager::process()
{
	if (warmupCounter < warmupLimitCount)
	{
		++warmupCounter;
	}
	else if (warmupCounter == warmupLimitCount) {
		updateFlag = true;
		++warmupCounter;
	}
}

void CollisionsManager::invalidate()
{
	runFlag = false;
	collisionDetectionThread.join();
}

void CollisionsManager::run()
{
	runFlag = true;
	updateFlag = false;
	collisionDetectionThread = std::thread(&CollisionsManager::processCollisions, this);
}

void CollisionsManager::processCollisions()
{
	while (runFlag)
	{
		if (updateFlag)
		{
			updateMutex.lock();

			checkAllCollisions();

			updateMutex.unlock();
		}
	}
}

void CollisionsManager::checkAllCollisions()
{
	for (int i = 0; i < collisionObjects.size() - 1; ++i)
	{
		for (int j = i + 1; j < collisionObjects.size(); ++j)
		{
			ObjectCollision& colObj1 = static_cast<ObjectCollision&>(*collisionObjects[i]);
			ObjectCollision& colObj2 = static_cast<ObjectCollision&>(*collisionObjects[j]);

			for (int k = 0; k < colObj1.objectHitboxes.size(); ++k)
			{
				HitboxObject& hitbox1 = static_cast<HitboxObject&>(*colObj1.objectHitboxes[k]);

				for (int l = 0; l < colObj2.objectHitboxes.size(); ++l)
				{
					HitboxObject& hitbox2 = static_cast<HitboxObject&>(*colObj2.objectHitboxes[l]);
					if (CollisionDetection::CheckCollision(*hitbox1.getHitbox(), *hitbox2.getHitbox()))
					{
						CollisionData data;
						data.objIdx = k;
						colObj1.addCollisionData(data);
						data.objIdx = l;
						colObj2.addCollisionData(data);
					}
				}
			}
		}
	}

	checkPointCollisions();
}

void CollisionsManager::checkPointCollisions()
{
	for (auto colPtRaw : collisionPoints)
	{
		for (auto colObjRaw : collisionObjects)
		{
			PointCollision& colPt = static_cast<PointCollision&>(*colPtRaw);
			ObjectCollision& colObj = static_cast<ObjectCollision&>(*colObjRaw);

			auto colPtVec = colPt.getCollisionPtVec();

			for (int i = 0; i < colPtVec.size(); ++i)
			{
				std::shared_ptr<glm::vec3> pt = colPtVec[i];
				for (int j = 0; j < colObj.objectHitboxes.size(); ++j)
				{
					HitboxObject& hitbox = static_cast<HitboxObject&>(*colObj.objectHitboxes[j]);
					if (CollisionDetection::CheckCollision(hitbox, *pt))
					{
						CollisionData data;
						data.objIdx = j;
						colObj.addCollisionData(data);
					}
				}
			}
		}
	}
}

void CollisionsManager::registerCollisionObject(GameObjectPtr object)
{
	object->addProperty<ObjectCollision>("collision");

	collisionObjects.push_back(static_cast<std::shared_ptr<Property<GameObject> > >(object->getProperty("collision")));
}

void CollisionsManager::registerCollisionPoints(GameObjectPtr object)
{
	object->addProperty<PointCollision>("point_collision");

	collisionPoints.push_back(static_cast<std::shared_ptr<Property<GameObject> >>(object->getProperty("point_collision")));
}

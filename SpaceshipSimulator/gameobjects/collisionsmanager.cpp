#include "collisionsmanager.h"

CollisionsManager::CollisionsManager()
	: runFlag(false)
	, updateFlag(false)
{}

void CollisionsManager::process()
{
	//if (updateFlag)
	//{
	//	updateMutex.lock();
	//	updateFlag = false;
	//	updateMutex.unlock();
	//}
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
		if (!updateFlag)
		{
			updateMutex.lock();
			//updateFlag = true;

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
						data.hitboxIdx = k;
						//colObj1.collisionData.push_back(data);
						colObj1.addCollisionData(data);
						data.hitboxIdx = l;
						colObj2.addCollisionData(data);
						//colObj2.collisionData.push_back(data);
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

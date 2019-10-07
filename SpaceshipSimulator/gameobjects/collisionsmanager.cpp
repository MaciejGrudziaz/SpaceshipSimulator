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
	else if (warmupCounter >= warmupLimitCount) {
		updateMutex.lock();
		updateFlag = true;
		updateMutex.unlock();
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

			updateFlag = false;

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

				if (hitbox1.isActive())
				{
					for (int l = 0; l < colObj2.objectHitboxes.size(); ++l)
					{
						HitboxObject& hitbox2 = static_cast<HitboxObject&>(*colObj2.objectHitboxes[l]);

						if (hitbox2.isActive())
						{
							if (CollisionDetection::CheckCollision(*hitbox1.getHitbox(), *hitbox2.getHitbox()))
							{
								glm::vec3 impactVector = (hitbox2.getGlobalPosition() - hitbox1.getGlobalPosition());
								impactVector = glm::normalize(impactVector);

								CollisionData data1(hitbox1, hitbox2);
								data1.collisionType = CollisionData::hitbox_crash;
								data1.impactVector = impactVector;
								colObj1.addCollisionData(data1);

								CollisionData data2(hitbox2, hitbox1);
								data2.collisionType = CollisionData::hitbox_crash;
								data2.impactVector = -impactVector;
								colObj2.addCollisionData(data2);
							}
						}
					}
				}
			}
		}
	}

	checkHitDetection();
}

void CollisionsManager::checkHitDetection()
{
	for (auto laserBeamRaw : hitDetectionObjects)
	{
		for (auto colObjRaw : collisionObjects)
		{
			LaserBeamHitDetection& laserBeamsObj = static_cast<LaserBeamHitDetection&>(*laserBeamRaw);
			ObjectCollision& colObj = static_cast<ObjectCollision&>(*colObjRaw);

			auto laserBeamsVec = laserBeamsObj.getLaserBeamsVec();

			for (int i = 0; i < laserBeamsVec.size(); ++i)
			{
				if (laserBeamsVec[i]->isActive())
				{
					glm::vec3 pt = laserBeamsVec[i]->getTransform().getPosition();
					for (int j = 0; j < colObj.objectHitboxes.size(); ++j)
					{
						HitboxObject& hitbox = static_cast<HitboxObject&>(*colObj.objectHitboxes[j]);
						if (hitbox.isActive())
						{
							if (CollisionDetection::CheckCollision(hitbox, pt))
							{
								CollisionData data1(hitbox, *laserBeamsVec[i]);
								data1.collisionType = CollisionData::laser_shot;
								colObj.addCollisionData(data1);

								CollisionData data2(*laserBeamsVec[i], hitbox);
								data2.collisionType = CollisionData::hitbox_crash;
								laserBeamsObj.addCollisionData(data2);
							}
						}
					}
				}
			}

			laserBeamsObj.releaseLaserBeamVec();
		}
	}
}

void CollisionsManager::registerCollisionObject(GameObjectPtr object)
{
	updateMutex.lock();

	object->addProperty<ObjectCollision>("collision");

	collisionObjects.push_back(static_cast<std::shared_ptr<Property<GameObject> > >(object->getProperty("collision")));

	updateMutex.unlock();
}

void CollisionsManager::registerHitDetectionObject(GameObjectPtr object)
{
	updateMutex.lock();

	object->addProperty<LaserBeamHitDetection>("hit_detection");

	hitDetectionObjects.push_back(static_cast<std::shared_ptr<Property<GameObject> >>(object->getProperty("hit_detection")));

	updateMutex.unlock();
}

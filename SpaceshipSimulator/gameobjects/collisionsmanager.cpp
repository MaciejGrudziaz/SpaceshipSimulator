#include "collisionsmanager.h"

CollisionsManager::CollisionsManager()
	: runFlag(false)
	, warmupCounter(0)
{}

void CollisionsManager::invalidate()
{
	runFlag = false;
	collisionDetectionThread.join();
}

void CollisionsManager::run()
{
	runFlag = true;
	collisionDetectionThread = std::thread(&CollisionsManager::processCollisions, this);
}

void CollisionsManager::processCollisions()
{
	while (runFlag)
	{
		checkHitboxCollisions();
		checkHitDetection();
	}
}

void CollisionsManager::checkHitboxCollisions()
{
	for (int i = 0; i < collisionObjects.size() - 1; ++i)
	{
		for (int j = i + 1; j < collisionObjects.size(); ++j)
		{
			ObjectCollision& colObj1 = static_cast<ObjectCollision&>(*collisionObjects[i]);
			ObjectCollision& colObj2 = static_cast<ObjectCollision&>(*collisionObjects[j]);
			auto colObj1Hitboxes = colObj1.getObjectHitboxes();
			auto colObj2Hitboxes = colObj2.getObjectHitboxes();

			for (int k = 0; k < colObj1Hitboxes.size(); ++k)
			{
				HitboxObject& hitbox1 = static_cast<HitboxObject&>(*colObj1Hitboxes[k]);

				if (hitbox1.isActive())
				{	
					for (int l = 0; l < colObj2Hitboxes.size(); ++l)
					{
						HitboxObject& hitbox2 = static_cast<HitboxObject&>(*colObj2Hitboxes[l]);

						if (hitbox2.isActive())
						{
							if (CollisionDetection::CheckCollision(*hitbox1.getHitbox(), *hitbox2.getHitbox()))
							{
								glm::vec3 impactVector = (hitbox2.getGlobalPosition() - hitbox1.getGlobalPosition());
								impactVector = glm::normalize(impactVector);

								CollisionDataPtr data1 = std::make_shared<CollisionData>(hitbox1, hitbox2);
								data1->collisionType = CollisionData::hitbox_crash;
								data1->impactVector = impactVector;
								colObj1.addCollisionData(data1);

								CollisionDataPtr data2 = std::make_shared<CollisionData>(hitbox2, hitbox1);
								data2->collisionType = CollisionData::hitbox_crash;
								data2->impactVector = -impactVector;
								colObj2.addCollisionData(data2);
							}
						}
					}
				}
			}
		}
	}
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
			auto colObjHitboxes = colObj.getObjectHitboxes();

			for (int j = 0; j < laserBeamsVec.size(); ++j)
			{
				if (laserBeamsVec[j]->isActive())
				{
					glm::vec3 pt = laserBeamsVec[j]->getTransform().getPosition();

					for (int k = 0; k < colObjHitboxes.size(); ++k)
					{
						HitboxObject& hitbox = static_cast<HitboxObject&>(*colObjHitboxes[k]);
						if (hitbox.isActive())
						{
							if (CollisionDetection::CheckCollision(hitbox, pt))
							{
								CollisionDataPtr data1 = std::make_shared<CollisionData>(hitbox, *laserBeamsVec[j]);
								data1->collisionType = CollisionData::laser_shot;
								colObj.addCollisionData(data1);

								CollisionDataPtr data2 = std::make_shared<CollisionData>(*laserBeamsVec[j], hitbox);
								data2->collisionType = CollisionData::hitbox_crash;
								laserBeamsObj.addCollisionData(data2);
							}
						}
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

void CollisionsManager::registerHitDetectionObject(GameObjectPtr object)
{
	object->addProperty<LaserBeamHitDetection>("hit_detection");

	hitDetectionObjects.push_back(static_cast<std::shared_ptr<Property<GameObject> >>(object->getProperty("hit_detection")));
}

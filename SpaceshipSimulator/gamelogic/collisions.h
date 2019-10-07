#pragma once

#include "../gameobjects/spaceship.h"
#include "../gameobjects/standardgameobject.h"
#include "collisiondetectionalgorithm.h"
#include "../gameobjects/hitboxobject.h"
#include "../gameobjects/asteroid.h"
#include "collisiondata.h"

class ObjectCollision : public Property<GameObject>
{
public:
	ObjectCollision(const std::string& name, GameObject& object);

	void init()override;
	void process()override;
	void invalidate()override;

	void addCollisionData(CollisionDataPtr data);
	void initCollisionDataCleanBuffer();

	std::vector<GameObjectPtr>& getObjectHitboxes();

private:
	std::vector<GameObjectPtr> objectHitboxes;
	std::vector<CollisionDataPtr> collisionData;

	static float laserShotDamage;
	bool initializedFlag;

	void loadHitboxObjects();
	HitboxObject& getHitboxObject(GameObject& obj);
	void resetHitboxes();

	int findFreeIndexInCollisionDataBuffer();
};

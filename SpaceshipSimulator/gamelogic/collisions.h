#pragma once

#include "../gameobjects/spaceship.h"
#include "../gameobjects/standardgameobject.h"
#include "collisiondetectionalgorithm.h"
#include "../gameobjects/hitboxobject.h"
#include "../gameobjects/asteroid.h"
#include "collisiondata.h"
#include <mutex>

class ObjectCollision : public Property<GameObject>
{
public:
	ObjectCollision(const std::string& name, GameObject& object);

	void init()override;
	void process()override;
	void invalidate()override;

	std::vector<GameObjectPtr> objectHitboxes;
	std::list<CollisionData> collisionData;

	void addCollisionData(const CollisionData& data);

private:
	static float laserShotDamage;
	std::mutex colDataMutex;
	bool initializedFlag;

	void loadHitboxObjects();
	HitboxObject& getHitboxObject(GameObject& obj);
	void resetHitboxes();
};

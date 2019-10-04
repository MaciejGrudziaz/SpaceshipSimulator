#pragma once

//#include "../gameengine.h"
//#include "../gameobjects/gameresources.h"
#include "../gameobjects/standardgameobject.h"
#include "collisiondetectionalgorithm.h"
#include "../gameobjects/hitboxobject.h"
#include <mutex>

struct CollisionData
{
	glm::vec3 impactVector;
	glm::vec3 reflectVector;
	
	float objectSpeed;
	float objectMass;

	int hitboxIdx;
};

class ObjectCollision : public Property<GameObject>
{
public:
	ObjectCollision(const std::string& name, GameObject& object);

	void init()override;
	void process()override;
	void invalidate()override;

	//static std::vector<GameObjectPtr> allHitboxes;
	std::vector<GameObjectPtr> objectHitboxes;
	std::list<CollisionData> collisionData;
	float collisionRadius;

	void addCollisionData(const CollisionData& data);

private:
	void loadHitboxObjects();
	void calcCollisionRadius();
	HitboxObject& getHitboxObject(GameObjectPtr obj);
	bool isHitboxExternal(GameObjectPtr obj);
	void resetHitboxes();

	std::mutex colDataMutex;
};

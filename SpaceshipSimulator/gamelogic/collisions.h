#pragma once

#include "../gameengine.h"
#include "../gameobjects/gameresources.h"
#include "collisiondetectionalgorithm.h"

struct CollisionData
{
	glm::vec3 impactVector;
	glm::vec3 reflectVector;
	
	float objectSpeed;
	float objectMass;
};

class ObjectCollision : public Property<GameObject>
{
public:
	ObjectCollision(const std::string& name, GameObject& object);

	void init()override;
	void process()override;
	void invalidate()override;

	static std::vector<GameObjectPtr> allHitboxes;
	std::vector<GameObjectPtr> objectHitboxes;
	std::list<CollisionData> collisionData;
	float collisionRadius;

private:
	void loadHitboxObjects();
	void calcCollisionRadius();
	HitboxObject& getHitboxObject(GameObjectPtr obj);
	bool isHitboxExternal(GameObjectPtr obj);
	void resetHitboxes();
};

void loadObjectsCollisionProperty(GameEngine& engine);

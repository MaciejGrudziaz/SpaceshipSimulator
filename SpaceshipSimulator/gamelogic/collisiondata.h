#pragma once

#include <glm/vec3.hpp>

struct CollisionData
{
	enum ColType { hitbox_crash, laser_shot };

	glm::vec3 impactVector;

	float collisionEnergy;

	GameObject& internalCollisionObj;				//game object who belongs to current collision property
	GameObject& externalCollisionObj;				//game object with which collision occured (doesn't belong to the same ObjectCollision property)

	ColType collisionType;

	bool isActive;

	CollisionData(GameObject& internalObject, GameObject& externalObject)
		: internalCollisionObj(internalObject)
		, externalCollisionObj(externalObject)
		, isActive(true)
	{}
};

typedef std::shared_ptr<CollisionData> CollisionDataPtr;

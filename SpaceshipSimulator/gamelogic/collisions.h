#pragma once

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

	int objIdx;
};

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
	void loadHitboxObjects();
	HitboxObject& getHitboxObject(GameObjectPtr obj);
	void resetHitboxes();

	std::mutex colDataMutex;
};

class PointCollision : public Property<GameObject>
{
public:
	PointCollision(const std::string& name, GameObject& object);

	void init()override;
	void process()override;
	void invalidate()override;

	void addCollisionPoint(std::shared_ptr<glm::vec3> pt);
	std::vector<std::shared_ptr<glm::vec3> >& getCollisionPtVec();
	void releaseCollisionPtVec();
	
	void addCollisionData(const CollisionData& data);

private:
	std::vector<std::shared_ptr<glm::vec3> > collisionPoints;
	std::list<CollisionData> collisionData;

	std::mutex colDataMutex;
	std::mutex colPtMutex;
	bool colPtVecAvailableFlag;
};

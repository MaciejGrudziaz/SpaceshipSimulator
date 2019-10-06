#include "collisions.h"

ObjectCollision::ObjectCollision(const std::string& name, GameObject& object)
	: Property<GameObject>(name, object)
{}

void ObjectCollision::init()
{
	loadHitboxObjects();
}

void ObjectCollision::loadHitboxObjects()
{
	StandardGameObject& standardObj = static_cast<StandardGameObject&>(object);

	std::list<std::string> childrenNames = object.getChildrenNames();

	std::for_each(childrenNames.begin(), childrenNames.end(), [this](auto childName) {
		size_t pos = childName.find_first_of('_');
		if (pos != std::string::npos)
		{
			if (childName.substr(0, pos) == "hitbox")
			{
				this->objectHitboxes.push_back(this->object.getChild(childName));
			}
		}
	});
}

void ObjectCollision::process()
{
	resetHitboxes();

	if (!collisionData.empty())
	{
		colDataMutex.lock();

		for (auto data : collisionData)
		{
			getHitboxObject(objectHitboxes[data.objIdx]).setHitboxColor(glm::vec3(0.0f, 1.0f, 0.0f));
		}

		StandardGameObject& standardObj = static_cast<StandardGameObject&>(object);
		standardObj.dealDamage(25);

		collisionData.clear();
		colDataMutex.unlock();
	}
}

void ObjectCollision::resetHitboxes()
{
	for (auto hitbox : objectHitboxes)
	{
		getHitboxObject(hitbox).setHitboxColor(glm::vec3(1.0f, 0.0f, 0.0f));
	}
}

HitboxObject& ObjectCollision::getHitboxObject(GameObjectPtr obj)
{
	return static_cast<HitboxObject&>(*obj);
}

void ObjectCollision::invalidate()
{}

void ObjectCollision::addCollisionData(const CollisionData& data)
{
	colDataMutex.lock();
	collisionData.push_back(data);
	colDataMutex.unlock();
}

PointCollision::PointCollision(const std::string& name, GameObject& object)
	: Property<GameObject>(name, object)
	, colPtVecAvailableFlag(true)
{}

void PointCollision::init()
{

}

void PointCollision::process()
{

}

void PointCollision::invalidate()
{}

void PointCollision::addCollisionPoint(std::shared_ptr<glm::vec3> pt)
{
	colPtMutex.lock();
	collisionPoints.push_back(pt);
	colPtMutex.unlock();
}

std::vector<std::shared_ptr<glm::vec3> >& PointCollision::getCollisionPtVec()
{
	colPtMutex.lock();
	colPtVecAvailableFlag = false;
	colPtMutex.unlock();

	return collisionPoints;
}

void PointCollision::releaseCollisionPtVec()
{
	colPtMutex.lock();
	colPtVecAvailableFlag = true;
	colPtMutex.unlock();
}

void PointCollision::addCollisionData(const CollisionData& data)
{
	colDataMutex.lock();
	collisionData.push_back(data);
	colDataMutex.unlock();
}

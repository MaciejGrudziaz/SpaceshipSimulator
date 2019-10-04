#include "collisions.h"

ObjectCollision::ObjectCollision(const std::string& name, GameObject& object)
	: Property<GameObject>(name, object)
	, collisionRadius(0.0f)
{}

void ObjectCollision::init()
{
	loadHitboxObjects();

	calcCollisionRadius();
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
				//this->allHitboxes.push_back(this->object.getChild(childName));
			}
		}
	});
}

void ObjectCollision::calcCollisionRadius()
{
	glm::vec3 maxPt;
	std::for_each(objectHitboxes.begin(), objectHitboxes.end(), [&maxPt](auto hitbox) {
		HitboxObject& hitboxObj = static_cast<HitboxObject&>(*hitbox);

		glm::vec3 maxCurrVal;
		for (int i = 0; i < 8; ++i)
		{
			for (int j = 0; j < 3; ++j)
			{
				if (hitboxObj.getHitbox()->basicVertices[i][j] > maxCurrVal[j])
					maxCurrVal[j] = hitboxObj.getHitbox()->basicVertices[i][j];
			}
		}

		maxCurrVal = hitboxObj.getTransform().getTransformMat() * glm::vec4(maxCurrVal, 1.0f);
		for (int i = 0; i < 3; ++i)
		{
			if (maxCurrVal[i] > maxPt[i])
				maxPt[i] = maxCurrVal[i];
		}
	});

	maxPt = object.getTransform().getScaleMat() * glm::vec4(maxPt, 1.0f);

	collisionRadius = glm::length(maxPt);
}

void ObjectCollision::process()
{
	//resetHitboxes();

	//for (auto hitbox : allHitboxes)
	//{
	//	if (isHitboxExternal(hitbox))
	//	{
	//		HitboxObject& hitbox1 = getHitboxObject(hitbox);
	//		for (auto objHitbox : objectHitboxes)
	//		{
	//			HitboxObject& hitbox2 = getHitboxObject(objHitbox);

	//			if (CollisionDetection::CheckCollision(*hitbox1.getHitbox(), *hitbox2.getHitbox()))
	//			{
	//				hitbox2.setHitboxColor(glm::vec3(0.0f, 1.0f, 0.0f));
	//			}
	//		}	
	//	}
	//}
	resetHitboxes();

	if (!collisionData.empty())
	{
		colDataMutex.lock();

		for (auto data : collisionData)
		{
			getHitboxObject(objectHitboxes[data.hitboxIdx]).setHitboxColor(glm::vec3(0.0f, 1.0f, 0.0f));
		}

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

bool ObjectCollision::isHitboxExternal(GameObjectPtr obj)
{
	bool isExternal = true;

	for (auto hitbox : objectHitboxes)
	{
		if (hitbox.get() == obj.get())
		{
			isExternal = false;
			break;
		}
	}

	return isExternal;
}

void ObjectCollision::invalidate()
{

}

void ObjectCollision::addCollisionData(const CollisionData& data)
{
	colDataMutex.lock();
	collisionData.push_back(data);
	colDataMutex.unlock();
}

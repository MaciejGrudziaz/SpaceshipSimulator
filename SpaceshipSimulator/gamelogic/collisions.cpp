#include "collisions.h"

float ObjectCollision::laserShotDamage = 0.0f;

ObjectCollision::ObjectCollision(const std::string& name, GameObject& object)
	: Property<GameObject>(name, object)
	, initializedFlag(false)
{}

void ObjectCollision::init()
{
	if (object.getName() == "spaceship")
	{
		Spaceship& spaceship = static_cast<Spaceship&>(object);
		laserShotDamage = spaceship.getShotDamage();
	}

	loadHitboxObjects();
	initializedFlag = true;
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
	if (!initializedFlag)
	{
		loadHitboxObjects();
		initializedFlag = true;
	}

	resetHitboxes();

	if (!collisionData.empty())
	{
		colDataMutex.lock();

		bool collisionWithSpaceship = false;
		bool laserShotHit = false;
		glm::vec3 impactVector;
		int hitboxColTotalCount = 0;
		for (auto data : collisionData)
		{
			getHitboxObject(data.internalCollisionObj).setHitboxColor(glm::vec3(0.0f, 1.0f, 0.0f));

			if (data.collisionType == CollisionData::laser_shot)
			{
				laserShotHit = true;
			}
			else {
				++hitboxColTotalCount;

				impactVector += data.impactVector;

				std::string colHitboxNameSuffix = data.externalCollisionObj.getName();
				size_t pos = colHitboxNameSuffix.find_last_of('_');
				colHitboxNameSuffix = colHitboxNameSuffix.substr(pos + 1);
				if (colHitboxNameSuffix == "spaceship")
					collisionWithSpaceship = true;
			}
		}

		if (hitboxColTotalCount > 0)
		{
			impactVector /= static_cast<float>(hitboxColTotalCount);

			if (object.getName() != "spaceship")
			{
				Asteroid& asteroid = static_cast<Asteroid&>(object);
				float size = asteroid.getTransform().getScale().x;
				float sizeFactor = 1.0f / size;
				glm::vec3 newSpeed = glm::normalize(asteroid.getLinearSpeed() - impactVector * sizeFactor  
									+ glm::vec3(0.0f,-1.0f,0.0f)) * glm::length(asteroid.getLinearSpeed());
				newSpeed.z = 0.0f;
				asteroid.setLinearSpeed(newSpeed);
			}
			else {
				StandardGameObject& standardObj = static_cast<StandardGameObject&>(object);
				standardObj.dealDamage(10.0f);
			}
		}

		if (laserShotHit)
		{
			if (object.getName() != "spaceship")
			{
				StandardGameObject& standardObj = static_cast<StandardGameObject&>(object);
				standardObj.dealDamage(laserShotDamage);
			}
		}

		if (collisionWithSpaceship)
		{
			StandardGameObject& standardObj = static_cast<StandardGameObject&>(object);
			standardObj.dealDamage(25);
		}

		collisionData.clear();
		colDataMutex.unlock();
	}
}

void ObjectCollision::resetHitboxes()
{
	for (auto hitbox : objectHitboxes)
	{
		getHitboxObject(*hitbox).setHitboxColor(glm::vec3(1.0f, 0.0f, 0.0f));
	}
}

HitboxObject& ObjectCollision::getHitboxObject(GameObject& obj)
{
	return static_cast<HitboxObject&>(obj);
}

void ObjectCollision::invalidate()
{}

void ObjectCollision::addCollisionData(const CollisionData& data)
{
	colDataMutex.lock();
	collisionData.push_back(data);
	colDataMutex.unlock();
}

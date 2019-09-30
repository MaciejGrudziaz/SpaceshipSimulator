#pragma once

#include <ctime>
#include "../gameengine.h"
#include "../gameobjects/gameresources.h"

class RandomRotation : public Property<GameObject>
{
public:
	RandomRotation(const std::string& name, GameObject& object)
		: Property<GameObject>(name, object)
	{}

	void init()override;
	void process()override;
	void invalidate()override;

private:
	glm::vec3 rotSpeed;
};

class AsteroidMovement : public Property<GameObject>
{
public:
	AsteroidMovement(const std::string& name, GameObject& object)
		: Property<GameObject>(name, object)
	{}

	void init()override;
	void process()override;
	void invalidate()override;

private:
	float speed;
};

void registerAsteroidBehaviours(GameEngine& engine);

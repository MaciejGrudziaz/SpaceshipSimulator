#include "asteroidbehaviour.h"

void RandomRotation::init()
{
	rotSpeed.x = (static_cast<float>(rand() % 100 + 1) / 100.0f) * 20.0f;
	rotSpeed.y = (static_cast<float>(rand() % 100 + 1) / 100.0f) * 20.0f;
	rotSpeed.z = (static_cast<float>(rand() % 100 + 1) / 100.0f) * 20.0f;

	//object.getTransform().setPosition(glm::vec3(0.0f, 20.0f, 0.0f));
}

void RandomRotation::process()
{
	glm::vec3 rot = object.getTransform().getRotation();

	rot += rotSpeed * Time::deltaTime;

	for (int i = 0; i < 3; ++i)
	{
		if (rot[i] > 360.0f)
			rot[i] = rot[i] - 360.0f;
	}

	object.getTransform().setRotation(rot);
}

void RandomRotation::invalidate()
{}

void AsteroidMovement::init()
{
	speed = static_cast<float>((rand() % 1000)) / 100.0f;
}

void AsteroidMovement::process()
{
	glm::vec3 pos = object.getTransform().getPosition();

	pos.y -= speed * Time::deltaTime;

	object.getTransform().setPosition(pos);
}

void AsteroidMovement::invalidate()
{}

void registerAsteroidBehaviours(GameEngine& engine)
{
	//engine.getResources()->asteroid->addProperty<RandomRotation>("rotation");
	std::for_each(engine.getResources()->asteroids.begin(), engine.getResources()->asteroids.end(), [](auto asteroid)
	{
		asteroid->addProperty<RandomRotation>("rotation");
		asteroid->addProperty<AsteroidMovement>("movement");
	});
}

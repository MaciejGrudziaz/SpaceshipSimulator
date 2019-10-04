#include "asteroid.h"

Asteroid::Asteroid()
{}

//void Asteroid::init(const glm::vec3& position, const glm::vec3& rotSpeed, const glm::vec3& linSpeed, float scale)
//{
//	this->rotSpeed = rotSpeed;
//	this->linearSpeed = linSpeed;
//
//	transform.setPosition(position);
//	transform.setScale(glm::vec3(scale,scale,scale));
//}

void Asteroid::process()
{
	StandardGameObject::process();

	if (isActive())
	{
		glm::vec3 pos = transform.getPosition();
		glm::vec3 rot = transform.getRotation();

		glm::vec3 worldSpeedVec(0.0f, -(*worldSpeed), 0.0f);
		pos += (linearSpeed + worldSpeedVec) * Time::deltaTime;
		rot += rotSpeed * Time::deltaTime;

		for (int i = 0; i < 3; ++i)
		{
			if (rot[i] > 360.0f) rot[i] = rot[i] - 360.0f;
			if (rot[i] < -360.0f) rot[i] = rot[i] + 360.0f;
		}

		transform.setPosition(pos);
		transform.setRotation(rot);
	}
}

void Asteroid::deepCopy(const Asteroid& asteroid)
{
	StandardGameObject::deepCopy(asteroid);

	this->worldSpeed = asteroid.worldSpeed;
	this->rotSpeed = asteroid.rotSpeed;
	this->linearSpeed = asteroid.linearSpeed;
}

void Asteroid::registerWorldSpeed(std::shared_ptr<float> speed)
{
	worldSpeed = speed;
}

void Asteroid::setRotSpeed(glm::vec3 rotSpeed)
{
	this->rotSpeed = rotSpeed;
}

void Asteroid::setLinearSpeed(glm::vec3 linSpeed)
{
	this->linearSpeed = linSpeed;
}
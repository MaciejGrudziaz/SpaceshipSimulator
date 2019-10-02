#pragma once

#include <glm/vec3.hpp>

struct Particle
{
	glm::vec3 pos;
	glm::vec3 speed;
	glm::vec3 color;
	float lifeTime;
	float maxLifeTime;
	float distanceFromCamera;
	float size;

	Particle()
		: pos(0.0f)
		, speed(0.0f)
		, color(0.0f)
		, lifeTime(0.0f)
		, size(1.0f)
		, maxLifeTime(0.0f)
		, distanceFromCamera(0.0f)
	{}

	void update(float dt)
	{
		if (lifeTime > 0.0f)
		{
			lifeTime -= dt;
			pos += speed * dt;
		}

		if (lifeTime < 0.0f)
			lifeTime = 0.0f;
	}

	bool operator<(const Particle& particle)
	{
		if (this->distanceFromCamera > particle.distanceFromCamera)
			return true;
		else return false;
	}
};


#pragma once

#include <glm/vec3.hpp>

struct Particle
{
	//glm::vec3 pos;
	float** pos;
	glm::vec3 speed;
	//glm::vec3 color;
	float** color;

	float lifeTime;
	float maxLifeTime;
	float distanceFromCamera;
	float* size;

	Particle()
		//: pos(0.0f)
		: speed(0.0f)
		//, color(0.0f)
		, lifeTime(0.0f)
		//, size(1.0f)
		, maxLifeTime(0.0f)
		, distanceFromCamera(0.0f)
	{
		pos = new float*[3];
		color = new float*[3];
	}
	~Particle()
	{
		delete pos;
		delete color;
	}

	void update(float dt)
	{
		if (lifeTime > 0.0f)
		{
			lifeTime -= dt;
			//pos += speed * dt;
			*pos[0] += speed.x * dt;
			*pos[1] += speed.y * dt;
			*pos[2] += speed.z * dt;
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

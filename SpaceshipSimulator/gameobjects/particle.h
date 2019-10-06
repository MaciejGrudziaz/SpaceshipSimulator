#pragma once

#include <glm/vec3.hpp>
#include <memory>

struct Particle
{
	//glm::vec3 pos;
	float** pos;
	glm::vec3 speed;
	//glm::vec3 color;
	float** color;

	float lifeTimeFrac;
	float lifeTime;
	float maxLifeTime;
	float distanceFromCamera;
	float* size;

	Particle();
	~Particle();

	void linkPosWithParticlesBuffer(float* posX, float* posY, float* posZ);

	void linkSizeWithParticlesBuffer(float* size);

	void linkColorWithParticlesBuffer(float* colorR, float* colorG, float* colorB, float* colorA);

	void update(float dt, const glm::vec3& baseColor, const glm::vec3& destColor);

	void calcDistance(glm::vec3 cameraPos)
	{
		distanceFromCamera = 0.0f;
		distanceFromCamera += std::abs(cameraPos.x - *pos[0]);
		distanceFromCamera += std::abs(cameraPos.y - *pos[1]);
		distanceFromCamera += std::abs(cameraPos.z - *pos[2]);
	}

	bool operator<(const Particle& particle)
	{
		if (this->distanceFromCamera > particle.distanceFromCamera)
			return true;
		else return false;
	}
};

typedef std::shared_ptr<Particle> ParticlePtr;

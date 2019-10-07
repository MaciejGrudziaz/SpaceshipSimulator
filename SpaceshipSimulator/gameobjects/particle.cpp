#include "particle.h"

#ifdef OLD_VERSION

Particle::Particle()
//: pos(0.0f)
	: speed(0.0f)
	//, color(0.0f)
	, lifeTime(0.0f)
	//, size(1.0f)
	, maxLifeTime(0.0f)
	, distanceFromCamera(0.0f)
{
	pos = new float*[3];
	color = new float*[4];
}
Particle::~Particle()
{
	delete pos;
	delete color;
}

void Particle::linkPosWithParticlesBuffer(float* posX, float* posY, float* posZ)
{
	pos[0] = posX;
	pos[1] = posY;
	pos[2] = posZ;
}

void Particle::linkSizeWithParticlesBuffer(float* size)
{
	this->size = size;
}

void Particle::linkColorWithParticlesBuffer(float* colorR, float* colorG, float* colorB, float* colorA)
{
	color[0] = colorR;
	color[1] = colorG;
	color[2] = colorB;
	color[3] = colorA;
}

void Particle::update(float dt, const glm::vec3& baseColor, const glm::vec3& destColor)
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

	lifeTimeFrac = lifeTime / maxLifeTime;

	*color[0] = baseColor.x + (destColor.x - baseColor.x) * (1.0f - lifeTimeFrac);
	*color[1] = baseColor.y + (destColor.y - baseColor.y) * (1.0f - lifeTimeFrac);
	*color[2] = baseColor.z + (destColor.z - baseColor.z) * (1.0f - lifeTimeFrac);
	*color[3] = lifeTimeFrac;
}

#endif

#pragma once

#include <GameRenderer/texturerenderobject.h>

struct Particle
{
	glm::vec3 pos;
	float lifeTime;
	float maxLifeTime;
	glm::vec3 speed;
	float distanceFromCamera;
	float size;

	Particle()
		: pos(0.0f)
		, lifeTime(0.0f)
		, speed(0.0f)
		, size(1.0f)
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
		//return (this->distanceFromCamera > particle.distanceFromCamera);
		if (this->distanceFromCamera > particle.distanceFromCamera)
			return true;
		else return false;
	}
};

class ParticleRenderer : public TextureRenderObject
{
public:

	ParticleRenderer(std::vector<Particle>& particles);

	void init()override;
	void process()override;
	void invalidate()override;

	void loadBuffers();

	void setParticlesCount(int val);

private:
	GLuint particlePositionSizeBuffer;
	GLuint particleColorBuffer;
	int particlesCount;

	GLint attribVertex;
	GLint attribPosSize;
	GLint attribColor;

	std::vector<Particle>& particles;
	std::vector<float> particlesPositionSize;
	std::vector<float> particlesColor;

	void bindBuffers();
	void updateBuffers();

};

typedef std::shared_ptr<ParticleRenderer> ParticleRendererPtr;

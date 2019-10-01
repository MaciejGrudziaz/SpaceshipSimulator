#pragma once

#include <GameRenderer/texturerenderobject.h>
#include "../renderers/particlerenderer.h"
#include "../gameobjects/modelimportstruct.h"
#include "../utilities/time.h"
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/norm.hpp>

class ParticleSystem
{
public:
	ParticleSystem();

	void setParticlesCount(int count);
	void setParticlesOrigin(const glm::vec3& pos);
	void setParticlesDirection(const glm::vec3& direction);
	void setParticlesMaxSpeed(float val);
	void setParticlesMaxLifetime(float val);
	void setParticlesSize(float val);
	void setParticlesSpreadCone(float minAngle, float maxAngle);

	int getParticlesCount()const;
	glm::vec3 getParticlesOrigin()const;
	glm::vec3 getParticlesDirection()const;
	float getParticlesMaxSpeed()const;
	float getParticlesMaxLifetime()const;
	float getParticlesSize()const;
	glm::vec2 getParticlesSpreadCone()const;

	void load(const ParticleSystemData& data, const ModelExternalUniforms& uniforms);
	void update(const glm::vec3& cameraPos);

	ParticleRendererPtr getRenderer()const;

private:
	std::mt19937 rng;
	glm::vec3 origin;
	glm::vec3 directionVec;
	glm::vec2 spreadCone;
	float maxSpeed;
	int particlesCount;
	float maxLifetime;
	float size;

	ParticleRendererPtr renderer;
	std::vector<Particle> particles;

	void createParticles();
	void initUniforms(const ModelExternalUniforms& uniforms);
	void initAttribPointers();
	void loadParticlesBuffer();

	void setParticleRandomData(Particle& particle);
	int findFirstDeadParticle(int idx);
};

typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;

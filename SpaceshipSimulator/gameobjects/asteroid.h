#pragma once

#include "particlesystem.h"
#include "standardgameobject.h"
#include "../utilities/time.h"

class Asteroid: public StandardGameObject
{
public:
	Asteroid();

	void init()override;
	void process()override;
	void invalidate()override;

	void initParticleSystems();

	void deepCopy(const Asteroid& asteroid);

	virtual void setActive(bool val)override;

	void registerWorldSpeed(std::shared_ptr<float> speed);
	void registerCamera(CameraPtr camera);
	void registerProjectionMatrixPtr(ConstMat4Ptr projection);
	
	void setRotSpeed(glm::vec3 rotSpeed);
	void setLinearSpeed(glm::vec3 linSpeed);

	void dealDamage()override;

	RenderObjectPtr getExplosionParticlesRenderer()const;
	RenderObjectPtr getExplosionFragmentsParticlesRenderer()const;

	bool isParticleSystemRunning()const;

private:
	glm::vec3 rotSpeed;
	glm::vec3 linearSpeed;
	std::shared_ptr<float> worldSpeed;
	ConstMat4Ptr projection;
	CameraPtr camera;

	ParticleSystemPtr explosionParticles;
	ParticleSystemPtr explosionFragmentsParticles;
};

typedef std::shared_ptr<Asteroid> AsteroidPtr;

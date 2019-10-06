#pragma once

#include "particlestruct.h"
#include "particlesystem.h"
#include "particlesystemv2.h"
#include "standardgameobject.h"
#include "../utilities/time.h"

class Asteroid: public StandardGameObject
{
public:
	Asteroid();

	void init()override;
	void process()override;
	void invalidate()override;

	void deepCopy(const Asteroid& asteroid);

	virtual void setActive(bool val)override;

	void registerWorldSpeed(std::shared_ptr<float> speed);
	void registerCamera(CameraPtr camera);
	void registerProjectionMatrixPtr(ConstMat4Ptr projection);
	
	void setRotSpeed(glm::vec3 rotSpeed);
	void setLinearSpeed(glm::vec3 linSpeed);

	void dealDamage(float val)override;

	void setHealth(float val);
	float getHealth()const;

	ParticleSystemDataPtr getExplosionParticleSystemData()const;
	ParticleSystemDataPtr getExplosionFragmentsParticleSystemData()const;
	//RenderObjectPtr getExplosionParticlesRenderer()const;
	//RenderObjectPtr getExplosionFragmentsParticlesRenderer()const;
	//ParticleSystemPtr getExplosionParticlesSystem()const;
	//ParticleSystemPtr getExplosionFragmentsParticlesSystem()const;

	bool isParticleSystemRunning()const;
	bool isDestroyed()const;

	void initParticleSystemsData(float explosionParticlesysMaxLifeTime, float explosionFragmentsParticlesysMaxLifeTime);

private:
	bool destroyedFlag;
	glm::vec3 rotSpeed;
	glm::vec3 linearSpeed;
	std::shared_ptr<float> worldSpeed;
	ConstMat4Ptr projection;
	CameraPtr camera;
	float health;

	//ParticleSystemV2Ptr explosionParticles;
	//ParticleSystemV2Ptr explosionFragmentsParticles;

	ParticleSystemDataPtr explosionParticleSystemData;
	ParticleSystemDataPtr explosionFragmentsParticleSystemData;
};

typedef std::shared_ptr<Asteroid> AsteroidPtr;

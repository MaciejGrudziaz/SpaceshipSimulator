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

	void setActive(bool val)override;
	void restart()override;

	void registerWorldSpeed(std::shared_ptr<float> speed);
	void registerCamera(CameraPtr camera);
	void registerProjectionMatrixPtr(ConstMat4Ptr projection);
	
	void setRotSpeed(glm::vec3 rotSpeed);

	glm::vec3 getLinearSpeed()const;
	void setLinearSpeed(glm::vec3 linSpeed);

	void dealDamage(float val)override;
	void destroy()override;

	void setHealth(float val);
	float getHealth()const;

	ParticleSystemDataPtr getExplosionParticleSystemData()const;
	ParticleSystemDataPtr getExplosionFragmentsParticleSystemData()const;

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

	bool asteroidCollisionAttachFlag;
	std::shared_ptr<glm::vec3> asteroidCollisionSharedSpeed;

	ParticleSystemDataPtr explosionParticleSystemData;
	ParticleSystemDataPtr explosionFragmentsParticleSystemData;

	void setHitboxActive(bool val);
};

typedef std::shared_ptr<Asteroid> AsteroidPtr;

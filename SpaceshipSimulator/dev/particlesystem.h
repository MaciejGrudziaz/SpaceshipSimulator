#pragma once

#include <GameRenderer/texturerenderobject.h>
#include <GameResources/gameobjects/gameobject.h>
#include <GameResources/gameobjects/camera.h>
#include "../renderers/particlerenderer.h"
#include "../gameobjects/modelimportstruct.h"
#include "../utilities/time.h"
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/norm.hpp>
#include <thread>
#include <mutex>
#include <array>

class ParticleSystem: public GameObject
{
public:
	ParticleSystem();

	void registerCamera(CameraPtr camera);

	void setParticlesCount(int count);
	void setParticlesMaxSpeed(float val);
	void setParticlesMaxLifetime(float val);
	void setParticlesSize(float val);
	void setParticlesSpreadCone(float minAngle, float maxAngle);
	void setColors(const glm::vec3& base, const glm::vec3& dest);

	int getParticlesCount()const;
	float getParticlesMaxSpeed()const;
	float getParticlesMaxLifetime()const;
	float getParticlesSize()const;
	glm::vec2 getParticlesSpreadCone()const;
	glm::vec3 getBaseColor()const;
	glm::vec3 getDestColor()const;

	void load(const ParticleSystemData& data, const ModelExternalUniforms& uniforms);

	virtual void init()override;
	virtual void process()override;
	virtual void invalidate()override;

	ParticleRendererPtr getRenderer()const;

private:
	std::mt19937 rng;
	glm::vec2 spreadCone;
	float maxSpeed;
	int particlesCount;
	float maxLifetime;
	float particleSize;
	glm::vec3 baseColor;
	glm::vec3 destColor;
	
	bool calc;
	std::mutex updateMut;
	std::mutex part1Update;
	std::mutex part2Update;
	std::mutex part3Update;
	std::mutex part4Update;

	std::array<bool, 5> updateStatus;
	//std::thread calcParticlesThreads[5];
	std::thread particleThread;


	CameraPtr camera;
	ParticleRendererPtr renderer;
	std::vector<Particle> particles;
	std::vector<float> particlesPosSizeBuffer;
	std::vector<float> particlesColorBuffer;
	int bufferVertexAttribSize;

	void createParticles();
	void initUniforms(const ModelExternalUniforms& uniforms);
	void setParticleRandomData(Particle& particle);
	void updateRendererBuffers();
	void createRendererBuffers();

	void processParticles();
	void processParticlesPart1();
	void processParticlesPart2();
	void processParticlesPart3();
	void processParticlesPart4();
	void sortParticles();
};

typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;

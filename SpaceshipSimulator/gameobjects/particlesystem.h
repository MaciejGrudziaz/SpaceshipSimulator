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

	void load(const ParticleSystemData& data, const ModelExternalUniforms& uniforms);

	virtual void init()override;
	virtual void process()override;
	virtual void invalidate()override;
	virtual void setActive(bool val)override;

	//void pause();
	//void run();
	void launch();

	ParticleRendererPtr getRenderer()const;

	void registerCamera(CameraPtr camera);

	void setParticlesCount(int count);
	void setParticlesMaxSpeed(float val);
	void setParticlesMaxLifetime(float val);
	void setParticlesSize(float val);
	void setParticlesSpreadCone(float minAngle, float maxAngle);
	void setColors(const glm::vec3& base, const glm::vec3& dest);
	void setBlendingFunctions(GLenum sfactor, GLenum dfactor);
	void setEvenSpread();
	void setSingleSpread();
	void setContinuousSpred();

	int getParticlesCount()const;
	float getParticlesMaxSpeed()const;
	float getParticlesMaxLifetime()const;
	float getParticlesSize()const;
	glm::vec2 getParticlesSpreadCone()const;
	glm::vec3 getBaseColor()const;
	glm::vec3 getDestColor()const;
	bool isRunning()const;

private:
	glm::mat4 modelTransform;

	std::mt19937 rng;			//RNG seed
	bool evenSpred;				//flag for spherical spread
	glm::vec2 spreadCone;		//x - min angle value, y - max angle value [angles]
	float maxSpeed;	
	int particlesCount;
	float maxLifetime;
	float particleSize;
	glm::vec3 baseColor;
	glm::vec3 destColor;
	BlendFunctions blendFunc;	//OpenGL blend functions
	bool continuous;			//flag for continuous creation of particles
	bool externalLaunchFlag;

	bool launched;
	bool endParticlesProcess;	//flag for closing partciles processing thread
	bool updateStatus;			//flag for updating particles data in seperate thread
	std::mutex updateMut;		//mutex for particles updating [single particles update in single logic refresh function]
	//bool isPauseMutexLocked;	//flag for checking if pause mutex is locked [so the user cant unlock not assigned mutex and don't have to wait for locking one]
	//std::mutex pauseMutex;		//mutex for pausing particles processing thread
	std::thread particleThread;	//particle processing thread

	std::vector<Particle> particles;
	std::vector<float> particlesPosSizeBuffer;	//buffer for renderer
	std::vector<float> particlesColorBuffer;	//buffer for renderer
	int bufferVertexAttribSize;					//shader attribute size for both above buffers

	CameraPtr camera;
	ParticleRendererPtr renderer;

	//void pause();
	//void run();

	void createParticles();
	void initUniforms(const ModelExternalUniforms& uniforms);
	void setParticleRandomData(Particle& particle);
	glm::vec3 getNormalVec(const glm::vec3& vec);
	void updateRendererBuffers();
	void createRendererBuffers();

	void processParticles();
};

typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;
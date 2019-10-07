#pragma once

#include <GameRenderer/texturerenderobject.h>
#include <GameResources/gameobjects/gameobject.h>
#include <GameResources/gameobjects/camera.h>
#include "../renderers/particlerendererv2.h"
#include "particlev2.h"
#include "particlestruct.h"
#include "modelimportstruct.h"
#include "../utilities/time.h"
#include <random>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/norm.hpp>
#include <thread>
#include <mutex>
#include <array>

class ParticleSystemV2 : public GameObject
{
public:
	ParticleSystemV2();
	~ParticleSystemV2();

	void loadRenderer(const ParticleSystemFiles& data, const ModelExternalUniforms& uniforms);

	virtual void init()override;
	virtual void process()override;
	virtual void invalidate()override;
	virtual void setActive(bool val)override;

	void launch();
	float shutDown();

	void registerCamera(CameraPtr camera);
	ParticleRendererV2Ptr getRenderer()const;
	std::vector<float>& getParticlesBuffer();
	bool isRunning()const;

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
	BlendFunctions getBlendFunctions()const;

private:
	std::mt19937 rng;			//RNG seed
	bool evenSpred;				//flag for spherical spread
	glm::vec2 spreadCone;		//x - min angle value, y - max angle value [angles]
	float maxSpeed;
	int particlesCount;
	float maxLifetime;
	float particleSize;
	BlendFunctions blendFunc;	//OpenGL blend functions
	bool launchFlag;
	bool shutingDownFlag;

	std::shared_ptr<glm::mat4> modelTransform;
	std::shared_ptr<int> continuous;			//flag for continuous mode [1 - continusous spread, 0 - single spread]
	std::shared_ptr<float> runTime;				//current runtime of particle system
	std::shared_ptr<glm::vec3> baseColor;		
	std::shared_ptr<glm::vec3> destColor;
	std::shared_ptr<float> shutDownTime;		//time point when the shutdown comman dwas issued to system

	std::list<ParticleV2Ptr> particles;
	std::vector<float> particlesBuffer;			//buffer for renderer
	const int bufferVertexAttribSize = 8;		//shader attribute size for both above buffers

	CameraPtr camera;
	ParticleRendererV2Ptr renderer;

	void createParticles();
	void recreateParticles();
	void initUniforms(const ModelExternalUniforms& uniforms);
	void setParticleRandomData(ParticleV2& particle);
	glm::vec3 getNormalVec(const glm::vec3& vec);
	void createRendererBuffers();
};

typedef std::shared_ptr<ParticleSystemV2> ParticleSystemV2Ptr;

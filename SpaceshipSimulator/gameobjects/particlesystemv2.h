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

	//void pause();
	//void run();
	void launch();

	ParticleRendererV2Ptr getRenderer()const;

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
	float shutDown();
	void setContinuousSpred();

	int getParticlesCount()const;
	float getParticlesMaxSpeed()const;
	float getParticlesMaxLifetime()const;
	float getParticlesSize()const;
	glm::vec2 getParticlesSpreadCone()const;
	glm::vec3 getBaseColor()const;
	glm::vec3 getDestColor()const;
	BlendFunctions getBlendFunctions()const;

	bool isRunning()const;

	std::vector<float>& getParticlesBuffer();

private:
	std::mt19937 rng;			//RNG seed
	bool evenSpred;				//flag for spherical spread
	glm::vec2 spreadCone;		//x - min angle value, y - max angle value [angles]
	float maxSpeed;
	int particlesCount;
	float maxLifetime;
	float particleSize;
	//glm::vec3 baseColor;
	//glm::vec3 destColor;
	BlendFunctions blendFunc;	//OpenGL blend functions
	//bool continuous;			//flag for continuous creation of particles
	bool launchFlag;
	bool shutingDownFlag;

	std::shared_ptr<glm::mat4> modelTransform;
	std::shared_ptr<int> continuous;
	std::shared_ptr<float> runTime;
	std::shared_ptr<glm::vec3> baseColor;
	std::shared_ptr<glm::vec3> destColor;
	std::shared_ptr<float> shutDownTime;
	//std::shared_ptr<glm::vec3> translate;

	std::list<ParticleV2Ptr> particles;
	std::vector<float> particlesBuffer;		//buffer for renderer
	const int bufferVertexAttribSize = 8;	//shader attribute size for both above buffers

	CameraPtr camera;
	ParticleRendererV2Ptr renderer;
	//void pause();
	//void run();

	//void processParticles();

	//void startThread();
	void createParticles();
	void recreateParticles();
	void initUniforms(const ModelExternalUniforms& uniforms);
	void setParticleRandomData(ParticleV2& particle);
	glm::vec3 getNormalVec(const glm::vec3& vec);
	void updateRendererBuffers();
	void createRendererBuffers();

	friend class ParticleSystemManager;
};

typedef std::shared_ptr<ParticleSystemV2> ParticleSystemV2Ptr;

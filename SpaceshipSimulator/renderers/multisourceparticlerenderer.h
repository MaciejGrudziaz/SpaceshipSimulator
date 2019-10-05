#pragma once

#include <GameRenderer/texturerenderobject.h>
#include "rendererstruct.h"

struct ParticleSysRenderParams
{
	std::vector<float>& particlesPositionSizeBuffer;
	std::vector<float>& particlesColorBuffer;
	int particleCount;
	GLuint posSizeBufferId;
	GLuint colorBufferId;
	BlendFunctions blendFunctions;
	bool isActive;

	ParticleSysRenderParams(std::vector<float>& particlePosSizeBuff, std::vector<float>& particleColorBuff)
		: particlesPositionSizeBuffer(particlePosSizeBuff)
		, particlesColorBuffer(particleColorBuff)
		, particleCount(0)
		, posSizeBufferId(0)
		, colorBufferId(0)
		, isActive(false)
	{}
};

typedef std::shared_ptr<ParticleSysRenderParams> ParticleSysRenderParamsPtr;

class MultiSourceParticleRenderer : public TextureRenderObject
{
public:
	MultiSourceParticleRenderer();

	void init()override;
	void process()override;
	void invalidate()override;

	void registerParticleSystem(ParticleSysRenderParamsPtr particleSys);

	void updatePositionBuffer();
	void updateColorBuffer();

private:
	std::vector<ParticleSysRenderParamsPtr> particleSysRenderParams;

	bool posSizeBufferUpdateFlag;
	bool colorBufferUpdateFlag;

	void loadBuffers();
	void bindParticleSysBuffer(ParticleSysRenderParamsPtr particleSys);
	void updateBuffers();
};

typedef std::shared_ptr<MultiSourceParticleRenderer> MultiSourceParticleRendererPtr;

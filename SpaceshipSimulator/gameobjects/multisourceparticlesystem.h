#pragma once

#include "particlesystem.h"
#include "../renderers/multisourceparticlerenderer.h"

struct ParticleSystemParams
{
	ParticleSystemPtr particles;
	ParticleSysRenderParamsPtr renderParams;
};

typedef std::shared_ptr<ParticleSystemParams> ParticleSystemParamsPtr;

class MultiSourceParticleSystem: public GameObject
{
public:
	MultiSourceParticleSystem();

	void init()override;
	void process()override;
	void invalidate()override;
	void setActive(bool val)override;

	void registerCamera(CameraPtr camera);

	void registerParticleSystem(ParticleSystemPtr particleSystem);

	void loadRenderer(const ParticleSystemData& data, const ModelExternalUniforms& uniforms);

	MultiSourceParticleRendererPtr getRenderer()const;

private:
	std::vector<ParticleSystemParamsPtr> particleSystems;
	MultiSourceParticleRendererPtr renderer;
	CameraPtr camera;

	void initUniforms(const ModelExternalUniforms& uniforms);
};

typedef std::shared_ptr<MultiSourceParticleSystem> MultiSourceParticleSystemPtr;

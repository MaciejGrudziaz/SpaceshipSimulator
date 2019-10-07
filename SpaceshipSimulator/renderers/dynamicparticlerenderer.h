#pragma once

#ifdef OLD_VERSION

#include "particlerenderer.h"

class DynamicParticleRenderer : public RenderObject
{
public:
	DynamicParticleRenderer()
	{}

	void init()
	{}

	void process()override;
	void invalidate()override;

	void registerParticleRenderer(ParticleRendererPtr renderer);

private:
	std::vector<ParticleRendererPtr> renderers;

	void clearVector();
};

typedef std::shared_ptr<DynamicParticleRenderer> DynamicParticleRendererPtr;

#endif

#pragma once

#include <GameRenderer/texturerenderobject.h>
#include "../gameobjects/particlestruct.h"
#include "particlerendererv2.h"

class MultiSourceParticleRenderer : public ParticleRendererV2
{
public:
	MultiSourceParticleRenderer(std::vector<float>& buffer, const BlendFunctions& blend);

	void process()override;

	void registerUniformsPointers(ParticleSystemDataPtr uniformData);
	void registerParticleSystemData(ParticleSystemDataPtr particleSystem);

private:
	ParticleSystemDataPtr uniformData;
	std::list<ParticleSystemDataPtr> particleSystemsData;

	void setUniformsData(ParticleSystemDataPtr particleSys);
};

typedef std::shared_ptr<MultiSourceParticleRenderer> MultiSourceParticleRendererPtr;

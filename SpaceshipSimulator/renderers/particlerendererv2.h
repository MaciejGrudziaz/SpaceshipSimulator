#pragma once

#include <GameRenderer/texturerenderobject.h>
#include "rendererstruct.h"

class ParticleRendererV2 : public TextureRenderObject
{
public:
	ParticleRendererV2(std::vector<float>& buffer, const BlendFunctions& blend);

	virtual void init()override;
	virtual void process()override;
	virtual void invalidate()override;

	void setParticlesCount(int val);

	void updateBuffer();

protected:
	GLuint particleBufferId;
	int particlesCount;
	const int vertexAttribCount = 8;

	GLint attribVertex;
	GLint attribPos;
	GLint attribSpeed;
	GLint attribSize;
	GLint attribLifeTime;

	BlendFunctions blendFunc;

	bool bufferUpdateFlag;
	std::vector<float>& particlesBuffer;

	void loadBuffers();
	void bindBuffers();
	void updateBuffers();

};

typedef std::shared_ptr<ParticleRendererV2> ParticleRendererV2Ptr;
#pragma once

#include <GameRenderer/texturerenderobject.h>
#include "../dev/particle.h"

class ParticleRenderer : public TextureRenderObject
{
public:

	ParticleRenderer(std::vector<float>& posSizeBuffer, std::vector<float>& colorBuffer);

	void init()override;
	void process()override;
	void invalidate()override;

	void loadBuffers();

	void setParticlesCount(int val);

	void updatePositionBuffer();
	void updateColorBuffer();

private:
	GLuint particlePositionSizeBuffer;
	GLuint particleColorBuffer;
	int particlesCount;

	GLint attribVertex;
	GLint attribPosSize;
	GLint attribColor;

	//std::vector<Particle>& particles;
	bool posSizeBufferUpdateFlag;
	bool colorBufferUpdateFlag;
	std::vector<float>& particlesPositionSize;
	std::vector<float>& particlesColor;

	void bindBuffers();
	void updateBuffers();

};

typedef std::shared_ptr<ParticleRenderer> ParticleRendererPtr;

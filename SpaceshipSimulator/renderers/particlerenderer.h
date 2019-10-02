#pragma once

#include <GameRenderer/texturerenderobject.h>
#include "../gameobjects/particle.h"

struct BlendFunctions
{
	GLenum sfactor;
	GLenum dfactor;

	BlendFunctions()
		: sfactor(GL_SRC_ALPHA)
		, dfactor(GL_ONE_MINUS_SRC_ALPHA)
	{}
};

class ParticleRenderer : public TextureRenderObject
{
public:

	ParticleRenderer(std::vector<float>& posSizeBuffer, std::vector<float>& colorBuffer, const BlendFunctions& blend);

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

	BlendFunctions blendFunc;

	bool posSizeBufferUpdateFlag;
	bool colorBufferUpdateFlag;
	std::vector<float>& particlesPositionSize;
	std::vector<float>& particlesColor;

	void bindBuffers();
	void updateBuffers();

};

typedef std::shared_ptr<ParticleRenderer> ParticleRendererPtr;

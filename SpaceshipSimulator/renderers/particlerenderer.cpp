#include "particlerenderer.h"

ParticleRenderer::ParticleRenderer(std::vector<float>& posSizeBuffer, std::vector<float>& colorBuffer, const BlendFunctions& blend)
	: particlesPositionSize(posSizeBuffer)
	, particlesColor(colorBuffer)
	, posSizeBufferUpdateFlag(false)
	, colorBufferUpdateFlag(false)
	, blendFunc(blend)
{}

void ParticleRenderer::init()
{
	TextureRenderObject::init();

	glGenBuffers(1, &particlePositionSizeBuffer);
	glGenBuffers(1, &particleColorBuffer);

	attribColor = glGetAttribLocation(shader->getProgram(), "color");
	attribVertex = glGetAttribLocation(shader->getProgram(), "vertex");
	attribPosSize = glGetAttribLocation(shader->getProgram(), "centerPosSize");

	loadBuffers();
}

void ParticleRenderer::loadBuffers()
{
	static const GLfloat vertexBuffer[] = {
		-0.5f, -0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		0.5f,  0.5f,  0.0f,
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionSizeBuffer);
	glBufferData(GL_ARRAY_BUFFER, particlesCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, particlesCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
}

void ParticleRenderer::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR && activeFlag)
	{
		glEnable(GL_BLEND);
		glBlendFunc(blendFunc.sfactor, blendFunc.dfactor);

		glUseProgram(shader->getProgram());

		bindBuffers();
		updateBuffers();

		bindTexture();
		updateUniforms();

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);

		glDisableVertexAttribArray(attribVertex);
		glDisableVertexAttribArray(attribPosSize);
		glDisableVertexAttribArray(attribColor);

		glDisable(GL_BLEND);

	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void ParticleRenderer::bindBuffers()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionSizeBuffer);
	glVertexAttribPointer(attribPosSize, 4, GL_FLOAT, GL_FALSE, 4*sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribPosSize);
	glEnableVertexAttribArray(attribColor);
}

void ParticleRenderer::updateBuffers()
{
	if (posSizeBufferUpdateFlag)
	{
		glBindBuffer(GL_ARRAY_BUFFER, particlePositionSizeBuffer);
		glBufferData(GL_ARRAY_BUFFER, particlesCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * 4 * sizeof(float), particlesPositionSize.data());

		posSizeBufferUpdateFlag = false;
	}

	if (colorBufferUpdateFlag)
	{
		glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
		glBufferData(GL_ARRAY_BUFFER, particlesCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * 4 * sizeof(float), particlesColor.data());

		colorBufferUpdateFlag = false;
	}
}	

void ParticleRenderer::invalidate()
{
	TextureRenderObject::invalidate();

	glDeleteBuffers(1, &particlePositionSizeBuffer);
	glDeleteBuffers(1, &particleColorBuffer);
}

void ParticleRenderer::setParticlesCount(int val)
{
	particlesCount = val;
}

void ParticleRenderer::updatePositionBuffer()
{
	posSizeBufferUpdateFlag = true;
}

void ParticleRenderer::updateColorBuffer()
{
	colorBufferUpdateFlag = true;
}

#include "multisourceparticlerenderer.h"

MultiSourceParticleRenderer::MultiSourceParticleRenderer()
	: posSizeBufferUpdateFlag(false)
	, colorBufferUpdateFlag(false)
{}

void MultiSourceParticleRenderer::init()
{
	TextureRenderObject::init();

	loadBuffers();
}

void MultiSourceParticleRenderer::loadBuffers()
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

	for (auto particleSys : particleSysRenderParams)
	{
		glBindBuffer(GL_ARRAY_BUFFER, particleSys->posSizeBufferId);
		glBufferData(GL_ARRAY_BUFFER, particleSys->particleCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

		glBindBuffer(GL_ARRAY_BUFFER, particleSys->colorBufferId);
		glBufferData(GL_ARRAY_BUFFER, particleSys->particleCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	}
}

void MultiSourceParticleRenderer::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR && activeFlag)
	{
		glEnable(GL_BLEND);

		glUseProgram(shader->getProgram());

		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);

		updateBuffers();
		updateUniforms();
		bindTexture();

		for (auto particleSys : particleSysRenderParams)
		{
			if (particleSys->isActive)
			{
				glBlendFunc(particleSys->blendFunctions.sfactor, particleSys->blendFunctions.dfactor);
				bindParticleSysBuffer(particleSys);
				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particleSys->particleCount);
			}
		}

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glDisable(GL_BLEND);

	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void MultiSourceParticleRenderer::bindParticleSysBuffer(ParticleSysRenderParamsPtr particleSys)
{
	glBindBuffer(GL_ARRAY_BUFFER, particleSys->posSizeBufferId);
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, particleSys->colorBufferId);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
}

void MultiSourceParticleRenderer::updateBuffers()
{
	if (posSizeBufferUpdateFlag)
	{
		for (auto particleSys : particleSysRenderParams)
		{
			glBindBuffer(GL_ARRAY_BUFFER, particleSys->posSizeBufferId);
			glBufferData(GL_ARRAY_BUFFER, particleSys->particleCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, particleSys->particleCount * 4 * sizeof(float), 
				particleSys->particlesPositionSizeBuffer.data());
		}
		posSizeBufferUpdateFlag = false;
	}
	if (colorBufferUpdateFlag)
	{
		for (auto particleSys : particleSysRenderParams)
		{
			glBindBuffer(GL_ARRAY_BUFFER, particleSys->colorBufferId);
			glBufferData(GL_ARRAY_BUFFER, particleSys->particleCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
			glBufferSubData(GL_ARRAY_BUFFER, 0, particleSys->particleCount * 4 * sizeof(float), 
				particleSys->particlesColorBuffer.data());
		}
		colorBufferUpdateFlag = false;
	}
}

void MultiSourceParticleRenderer::invalidate()
{
	TextureRenderObject::invalidate();

	for (auto particleSys : particleSysRenderParams)
	{
		glDeleteBuffers(1, &particleSys->posSizeBufferId);
		glDeleteBuffers(1, &particleSys->colorBufferId);
	}
}

void MultiSourceParticleRenderer::registerParticleSystem(ParticleSysRenderParamsPtr particleSys)
{
	particleSysRenderParams.push_back(particleSys);

	glCreateBuffers(1, &particleSys->posSizeBufferId);
	glCreateBuffers(1, &particleSys->colorBufferId);
}

void MultiSourceParticleRenderer::updatePositionBuffer()
{
	posSizeBufferUpdateFlag = true;
}

void MultiSourceParticleRenderer::updateColorBuffer()
{
	colorBufferUpdateFlag = true;
}
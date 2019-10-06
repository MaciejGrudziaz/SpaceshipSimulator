#include "multisourceparticlerenderer.h"

MultiSourceParticleRenderer::MultiSourceParticleRenderer(std::vector<float>& buffer, const BlendFunctions& blend)
	: ParticleRendererV2(buffer, blend)
{}

void MultiSourceParticleRenderer::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR && activeFlag)
	{
		glEnable(GL_BLEND);
		glBlendFunc(blendFunc.sfactor, blendFunc.dfactor);

		glUseProgram(shader->getProgram());

		bindBuffers();
		updateBuffers();

		bindTexture();

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);

		for (auto particleSys : particleSystemsData)
		{
			if (particleSys->launchFlag)
			{
				setUniformsData(particleSys);

				updateUniforms();

				glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);
			}
		}

		glDisableVertexAttribArray(attribVertex);
		glDisableVertexAttribArray(attribPos);
		glDisableVertexAttribArray(attribSpeed);
		glDisableVertexAttribArray(attribSize);
		glDisableVertexAttribArray(attribLifeTime);

		glDisable(GL_BLEND);

	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void MultiSourceParticleRenderer::setUniformsData(ParticleSystemDataPtr particleSys)
{
	*(uniformData->baseColor) = *(particleSys->baseColor);
	*(uniformData->continuous) = *(particleSys->continuous);
	*(uniformData->destColor) = *(particleSys->destColor);
	*(uniformData->modelTransform) = *(particleSys->modelTransform);
	*(uniformData->runTime) = *(particleSys->runTime);
	*(uniformData->shutDownTime) = *(particleSys->shutDownTime);
}

void MultiSourceParticleRenderer::registerUniformsPointers(ParticleSystemDataPtr uniformData)
{
	this->uniformData = uniformData;
}

void MultiSourceParticleRenderer::registerParticleSystemData(ParticleSystemDataPtr particleSystem)
{
	particleSystemsData.push_back(particleSystem);
}

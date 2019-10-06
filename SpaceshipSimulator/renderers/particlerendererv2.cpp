#include "particlerendererv2.h"

#include "particlerenderer.h"

ParticleRendererV2::ParticleRendererV2(std::vector<float>& buffer, const BlendFunctions& blend)
	: particlesBuffer(buffer)
	, bufferUpdateFlag(false)
	, blendFunc(blend)
{}

void ParticleRendererV2::init()
{
	TextureRenderObject::init();

	glGenBuffers(1, &particleBufferId);

	attribVertex = glGetAttribLocation(shader->getProgram(), "vertex");
	attribPos = glGetAttribLocation(shader->getProgram(), "centerPos");
	attribSpeed = glGetAttribLocation(shader->getProgram(), "speed");
	attribSize = glGetAttribLocation(shader->getProgram(), "size");
	attribLifeTime = glGetAttribLocation(shader->getProgram(), "lifeTime");

	loadBuffers();
}

void ParticleRendererV2::loadBuffers()
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

	glBindBuffer(GL_ARRAY_BUFFER, particleBufferId);
	glBufferData(GL_ARRAY_BUFFER, particlesCount * vertexAttribCount * sizeof(float), NULL, GL_STREAM_DRAW);

}

void ParticleRendererV2::process()
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
		glVertexAttribDivisor(3, 1);
		glVertexAttribDivisor(4, 1);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);

		glDisableVertexAttribArray(attribVertex);
		glDisableVertexAttribArray(attribPos);
		glDisableVertexAttribArray(attribSpeed);
		glDisableVertexAttribArray(attribSize);
		glDisableVertexAttribArray(attribLifeTime);

		glDisable(GL_BLEND);

	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void ParticleRendererV2::bindBuffers()
{
	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, particleBufferId);
	glVertexAttribPointer(attribSpeed, 3, GL_FLOAT, GL_FALSE, vertexAttribCount * sizeof(float), (void*)0);
	glVertexAttribPointer(attribPos, 3, GL_FLOAT, GL_FALSE, vertexAttribCount * sizeof(float), (void*)(3*sizeof(float)));
	glVertexAttribPointer(attribSize, 1, GL_FLOAT, GL_FALSE, vertexAttribCount * sizeof(float), (void*)(6 * sizeof(float)));
	glVertexAttribPointer(attribLifeTime, 1, GL_FLOAT, GL_FALSE, vertexAttribCount * sizeof(float), (void*)(7 * sizeof(float)));

	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribPos);
	glEnableVertexAttribArray(attribSpeed);
	glEnableVertexAttribArray(attribSize);
	glEnableVertexAttribArray(attribLifeTime);
}

void ParticleRendererV2::updateBuffers()
{
	if (bufferUpdateFlag)
	{
		glBindBuffer(GL_ARRAY_BUFFER, particleBufferId);
		glBufferData(GL_ARRAY_BUFFER, particlesCount * vertexAttribCount * sizeof(float), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * vertexAttribCount * sizeof(float), particlesBuffer.data());

		bufferUpdateFlag = false;
	}
}

void ParticleRendererV2::invalidate()
{
	TextureRenderObject::invalidate();

	glDeleteBuffers(1, &particleBufferId);
}

void ParticleRendererV2::setParticlesCount(int val)
{
	particlesCount = val;
}

void ParticleRendererV2::updateBuffer()
{
	bufferUpdateFlag = true;
}

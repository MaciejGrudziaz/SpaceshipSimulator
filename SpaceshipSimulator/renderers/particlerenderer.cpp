#include "particlerenderer.h"

ParticleRenderer::ParticleRenderer(std::vector<float>& posSizeBuffer, std::vector<float>& colorBuffer)
	: particlesPositionSize(posSizeBuffer)
	, particlesColor(colorBuffer)
	, posSizeBufferUpdateFlag(true)
	, colorBufferUpdateFlag(true)
{}

void ParticleRenderer::init()
{
	TextureRenderObject::init();

	glGenBuffers(1, &particlePositionSizeBuffer);
	glGenBuffers(1, &particleColorBuffer);

	attribColor = glGetAttribLocation(shader->getProgram(), "color");
	attribVertex = glGetAttribLocation(shader->getProgram(), "vertex");
	attribPosSize = glGetAttribLocation(shader->getProgram(), "centerPosSize");
}

void ParticleRenderer::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR && isActive)
	{
		bindBuffers();

		updateBuffers();

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_DST_ALPHA);

		glUseProgram(shader->getProgram());

		bindTexture();
		updateUniforms();

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, particlesCount);

		glDisableVertexAttribArray(attribVertex);
		glDisableVertexAttribArray(attribPosSize);
		glDisableVertexAttribArray(attribColor);

	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void ParticleRenderer::bindBuffers()
{
	
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(attribVertex, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionSizeBuffer);
	glVertexAttribPointer(attribPosSize, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
	glVertexAttribPointer(attribColor, 4, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glEnableVertexAttribArray(attribVertex);
	glEnableVertexAttribArray(attribPosSize);
	glEnableVertexAttribArray(attribColor);
}

void ParticleRenderer::updateBuffers()
{
	//std::sort(particles.begin(), particles.end());

	//glm::vec3 color(0.7f, 0.7f, 0.7f);

	//int counter = 0;
	//for (const Particle& particle : particles)
	//{
	//	if (counter < particlesCount)
	//	{
	//		memcpy(particlesPositionSize.data() + 4 * counter, glm::value_ptr(particle.pos), 3 * sizeof(float));
	//		particlesPositionSize[4 * counter + 3] = particle.size;

	//		memcpy(particlesColor.data() + 4 * counter, glm::value_ptr(color), 3 * sizeof(float));
	//		particlesColor[4 * counter + 3] = particle.lifeTime / particle.maxLifeTime;

	//		++counter;
	//	}
	//}

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

	for (int i = 0; i < particlesCount; ++i)
	{
		for (int j = 0; j < 4; ++j)
		{
			particlesPositionSize.push_back(0.0f);
			particlesColor.push_back(0.0f);
		}
	}
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

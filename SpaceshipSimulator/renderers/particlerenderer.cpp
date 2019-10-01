#include "particlerenderer.h"

ParticleRenderer::ParticleRenderer(std::vector<Particle>& particles)
	: particles(particles)
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

		//glEnable(GL_POINT_SPRITE);
		//glEnable(GL_PROGRAM_POINT_SIZE);
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

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
	static int test = 0;
	++test;

	std::sort(particles.begin(), particles.end());

	glm::vec3 color(1.0f, 0.0f, 0.0f);

	particlesPositionSize.clear();
	particlesColor.clear();

	int counter = 0;
	for (const Particle& particle : particles)
	{
		if (counter < particlesCount)
		{
			particlesPositionSize.push_back(particle.pos.x);
			particlesPositionSize.push_back(particle.pos.y);
			particlesPositionSize.push_back(particle.pos.z);
			particlesPositionSize.push_back(particle.size);

			particlesColor.push_back(color.x);
			particlesColor.push_back(color.y);
			particlesColor.push_back(color.z);
			particlesColor.push_back(particle.lifeTime/particle.maxLifeTime);

			++counter;
		}
	}
	if (counter != particlesCount)
	{
		for (int i = counter; i < particlesCount; ++i)
		{
			particlesColor.push_back(0.0f);
			particlesColor.push_back(0.0f);
			particlesColor.push_back(0.0f);
			particlesColor.push_back(0.0f);
		}
	}

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionSizeBuffer);
	glBufferData(GL_ARRAY_BUFFER, particlesCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * 4 * sizeof(float), particlesPositionSize.data());

	glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, particlesCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, particlesCount * 4 * sizeof(float), particlesColor.data());
}	

void ParticleRenderer::invalidate()
{
	TextureRenderObject::invalidate();

	glDeleteBuffers(1, &particlePositionSizeBuffer);
	glDeleteBuffers(1, &particleColorBuffer);
}

void ParticleRenderer::loadBuffers()
{
	static const GLfloat g_vertex_buffer_data[] = {
		-0.5f, -0.5f, 0.0f,
		0.5f, -0.5f, 0.0f,
		-0.5f, 0.5f, 0.0f,
		0.5f, 0.5f, 0.0f,
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particlePositionSizeBuffer);
	glBufferData(GL_ARRAY_BUFFER, particlesCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, particleColorBuffer);
	glBufferData(GL_ARRAY_BUFFER, particlesCount * 4 * sizeof(float), NULL, GL_STREAM_DRAW);
}

void ParticleRenderer::setParticlesCount(int val)
{
	particlesCount = val;
}

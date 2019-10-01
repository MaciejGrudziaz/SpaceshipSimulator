#include "particles.h"

ParticleSystem::ParticleSystem()
	: origin(0.0f)
	, directionVec(0.0f)
	, particlesCount(0)
	, size(1.0f)
	, spreadCone(-45.0f, 45.0f)
{}

void ParticleSystem::setParticlesCount(int count)
{
	particlesCount = count;
}

void ParticleSystem::setParticlesOrigin(const glm::vec3& pos)
{
	origin = pos;
}

void ParticleSystem::setParticlesDirection(const glm::vec3& direction)
{
	directionVec = direction;
}

void ParticleSystem::setParticlesMaxSpeed(float val)
{
	maxSpeed = val;
}

void ParticleSystem::setParticlesMaxLifetime(float val)
{
	maxLifetime = val;
}

void ParticleSystem::setParticlesSize(float val)
{
	size = val;
}

void ParticleSystem::setParticlesSpreadCone(float minAngle, float maxAngle)
{
	spreadCone.x = minAngle;
	spreadCone.y = maxAngle;
}

int ParticleSystem::getParticlesCount()const
{
	return particlesCount;
}

glm::vec3 ParticleSystem::getParticlesOrigin()const
{
	return origin;
}

glm::vec3 ParticleSystem::getParticlesDirection()const
{
	return directionVec;
}

float ParticleSystem::getParticlesMaxSpeed()const
{
	return maxSpeed;
}

float ParticleSystem::getParticlesMaxLifetime()const
{
	return maxLifetime;
}

float ParticleSystem::getParticlesSize()const
{
	return size;
}

glm::vec2 ParticleSystem::getParticlesSpreadCone()const
{
	return spreadCone;
}

void ParticleSystem::load(const ParticleSystemData& data, const ModelExternalUniforms& uniforms)
{
	createParticles();

	renderer = std::make_shared<ParticleRenderer>(particles);
	renderer->setParticlesCount(particlesCount);
	renderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	initUniforms(uniforms);
	renderer->init();
	renderer->loadBuffers();
	renderer->loadTexture(data.particleTexture);
}

void ParticleSystem::createParticles()
{
	for (int i = 0; i < particlesCount; ++i)
	{
		Particle particle;
		setParticleRandomData(particle);
		particles.push_back(particle);
	}
}

void ParticleSystem::setParticleRandomData(Particle& particle)
{
	std::uniform_real_distribution<> randLifetime(0.25 * maxLifetime, maxLifetime);
	std::uniform_real_distribution<> randSpeed(0.8 * maxSpeed, 1.2 * maxSpeed);
	std::uniform_real_distribution<> randRotAngle(spreadCone.x, spreadCone.y);
	std::uniform_real_distribution<> randAxis(0.0f, 1.0f);
	std::uniform_real_distribution<> fullRandAxis(-1.0f, 1.0f);

	particle.pos = origin;
	particle.size = size;
	particle.maxLifeTime = maxLifetime;
	particle.lifeTime = randLifetime(rng);
	particle.distanceFromCamera = 0.0f;

	if (directionVec != glm::vec3(0.0f))
	{
		glm::vec3 rotAxis(randAxis(rng), randAxis(rng), randAxis(rng));
		rotAxis = glm::normalize(rotAxis);
		particle.speed = static_cast<float>(randSpeed(rng)) * glm::normalize(glm::rotate(directionVec, glm::radians(static_cast<float>(randRotAngle(rng))), rotAxis));
	}
	else {
		particle.speed = static_cast<float>(randSpeed(rng)) * glm::normalize(glm::vec3(fullRandAxis(rng), fullRandAxis(rng), fullRandAxis(rng)));
	}
}


void ParticleSystem::initUniforms(const ModelExternalUniforms& uniforms)
{
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");

	viewUniform->mat = uniforms.view;
	projectionUniform->mat = uniforms.projection;

	renderer->addUniform(viewUniform);
	renderer->addUniform(projectionUniform);
}

void ParticleSystem::initAttribPointers()
{
	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	renderer->addShaderAttribute(posAttrib);

	ShaderAttribute colorAttrib;
	colorAttrib.location = 1;
	colorAttrib.size = 3;
	colorAttrib.offset = (void*)(3 * sizeof(float));

	renderer->addShaderAttribute(colorAttrib);
}

void ParticleSystem::loadParticlesBuffer()
{
	//glm::vec3 testColor(1.0f, 0.0f, 0.0f);

	//std::vector<float> buffer;

	//for (auto pt : particles)
	//{
	//	for (int i = 0; i < 3; ++i)
	//		buffer.push_back(pt[i]);

	//	for (int i = 0; i < 3; ++i)
	//		buffer.push_back(testColor[i]);
	//}

	//renderer->loadBuffer(buffer);
}

void ParticleSystem::update(const glm::vec3& cameraPos)
{
	for (Particle& particle : particles)
	{
		particle.update(Time::deltaTime);
		particle.distanceFromCamera = glm::length2(particle.pos - cameraPos);

		if (particle.lifeTime <= 0.0f)
			setParticleRandomData(particle);
	}
}

int ParticleSystem::findFirstDeadParticle(int idx)
{
	for (int i = idx; i < particles.size(); ++i)
	{
		if (particles[i].lifeTime <= 0.0f)
			return i;
	}

	return -1;
}

ParticleRendererPtr ParticleSystem::getRenderer()const
{
	return renderer;
}

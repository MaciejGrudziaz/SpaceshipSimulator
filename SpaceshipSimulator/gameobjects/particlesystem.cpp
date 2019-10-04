#include "particlesystem.h"

ParticleSystem::ParticleSystem()
	: modelTransform(1.0f)
	, particlesCount(0)
	, particleSize(1.0f)
	, spreadCone(-45.0f, 45.0f)
	, maxSpeed(0.0f)
	, maxLifetime(0.0f)
	, bufferVertexAttribSize(4)
	, baseColor(1.0f, 1.0f, 1.0f)
	, destColor(1.0f, 1.0f, 1.0f)
	, endParticlesProcess(false)
	, evenSpred(false)
	, continuous(true)
	, launched(false)
	, externalLaunchFlag(false)
{}

void ParticleSystem::load(const ParticleSystemData& data, const ModelExternalUniforms& uniforms)
{
	//createParticles();

	renderer = std::make_shared<ParticleRenderer>(particlesPosSizeBuffer, particlesColorBuffer, blendFunc);
	renderer->setParticlesCount(particlesCount);
	renderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	initUniforms(uniforms);
	renderer->init();
	renderer->loadBuffers();
	renderer->loadTexture(data.particleTexture);

	//updateRendererBuffers();

	updateStatus = true;
	//pause();
	particleThread = std::thread(&ParticleSystem::processParticles, this);
}

void ParticleSystem::createParticles()
{
	particles.clear();
	for (int i = 0; i < particlesCount; ++i)
	{
		Particle particle;
		setParticleRandomData(particle);
		particles.push_back(particle);
	}
}

void ParticleSystem::setParticleRandomData(Particle& particle)
{
	std::uniform_real_distribution<> randLifetime(0.0f, maxLifetime);
	std::uniform_real_distribution<> randSpeed(0.8 * maxSpeed, 1.2 * maxSpeed);
	std::uniform_real_distribution<> randRotAngle(spreadCone.x, spreadCone.y);
	std::uniform_real_distribution<> fullRandAxis(-1.0f, 1.0f);
	std::uniform_real_distribution<> randSize(0.5f * particleSize, 1.2 * particleSize);

	particle.pos = modelTransform * glm::vec4(0.0f,0.0f,0.0f,1.0f);
	particle.size = randSize(rng);
	particle.maxLifeTime = maxLifetime;
	particle.lifeTime = randLifetime(rng);
	particle.distanceFromCamera = 0.0f;

	if (!evenSpred)
	{
		glm::vec3 directionVec = parentTransform * glm::vec4(transform.getOrientation(),0.0f);
		glm::vec3 normalVec = getNormalVec(directionVec);
		glm::vec3 normalVec2 = glm::cross(normalVec, directionVec);
		glm::vec3 randVec = glm::rotate(directionVec, static_cast<float>(glm::radians(randRotAngle(rng))), normalVec)
							+ glm::rotate(directionVec, static_cast<float>(glm::radians(randRotAngle(rng))), normalVec2);
		particle.speed = static_cast<float>(randSpeed(rng)) * glm::normalize(randVec);
	}
	else {
		particle.speed = static_cast<float>(randSpeed(rng)) * glm::normalize(glm::vec3(fullRandAxis(rng), fullRandAxis(rng), fullRandAxis(rng)));
	}
}

glm::vec3 ParticleSystem::getNormalVec(const glm::vec3& vec)
{
	static glm::vec3 xVec(1.0f, 0.0f, 0.0f), yVec(0.0f, 1.0f, 0.0f), zVec(0.0f, 0.0f, 1.0f);

	if (vec != xVec)
	{
		return glm::normalize(glm::cross(xVec, vec));
	}
	else if (vec != yVec)
	{
		return  glm::normalize(glm::cross(yVec, vec));
	}
	else {
		return  glm::normalize(glm::cross(zVec, vec));
	}
}

void ParticleSystem::updateRendererBuffers()
{
	if (particlesPosSizeBuffer.size() == 0 || particlesPosSizeBuffer.size() == 0)
	{
		createRendererBuffers();
	}
	else {
		glm::vec3 color;
		float lifetimeFrac;
		for(int i = 0;i<particlesCount; ++i)
		{
			memcpy(particlesPosSizeBuffer.data() + bufferVertexAttribSize * i, glm::value_ptr(particles[i].pos), 3 * sizeof(float));
			particlesPosSizeBuffer[bufferVertexAttribSize * i + 3] = particles[i].size;

			lifetimeFrac = particles[i].lifeTime / particles[i].maxLifeTime;
			color = baseColor;
			color += (destColor - baseColor) * (1.0f - lifetimeFrac);
			
			memcpy(particlesColorBuffer.data() + bufferVertexAttribSize * i, glm::value_ptr(color), 3 * sizeof(float));
			particlesColorBuffer[bufferVertexAttribSize * i + 3] = lifetimeFrac;

			renderer->updatePositionBuffer();
			renderer->updateColorBuffer();
		}
	}
}

void ParticleSystem::createRendererBuffers()
{
	if (particlesPosSizeBuffer.size() == 0)
	{
		for (int i = 0; i < particlesCount; ++i)
		{
			particlesPosSizeBuffer.push_back(particles[i].pos.x);
			particlesPosSizeBuffer.push_back(particles[i].pos.y);
			particlesPosSizeBuffer.push_back(particles[i].pos.z);
			particlesPosSizeBuffer.push_back(particles[i].size);
		}
	}
	if (particlesColorBuffer.size() == 0)
	{
		glm::vec3 color;
		float lifetimeFrac;
		for (int i = 0; i < particlesCount; ++i)
		{
			lifetimeFrac = particles[i].lifeTime / particles[i].maxLifeTime;
			color = baseColor;
			color += (destColor - baseColor) * (1.0f - lifetimeFrac);

			particlesColorBuffer.push_back(color.r);
			particlesColorBuffer.push_back(color.g);
			particlesColorBuffer.push_back(color.b);
			particlesColorBuffer.push_back(lifetimeFrac);
		}
	}
}

void ParticleSystem::initUniforms(const ModelExternalUniforms& uniforms)
{
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");
	UniformDataVec3Ptr cameraUpUniform = std::make_shared<UniformDataVec3>("cameraUp");
	UniformDataVec3Ptr cameraRightUniform = std::make_shared<UniformDataVec3>("cameraRight");

	viewUniform->mat = uniforms.view;
	projectionUniform->mat = uniforms.projection;
	cameraUpUniform->vec = camera->getUpVecPtr();
	cameraRightUniform->vec = camera->getRightVecPtr();

	renderer->addUniform(viewUniform);
	renderer->addUniform(projectionUniform);
	renderer->addUniform(cameraUpUniform);
	renderer->addUniform(cameraRightUniform);
}

void ParticleSystem::init()
{
	GameObject::init();
}

void ParticleSystem::process()
{
	GameObject::process();

	modelTransform = parentTransform * getTransform().getTransformMat();

	if(updateStatus)
	{
		updateMut.lock();
		updateStatus = false;
		updateMut.unlock();
	}
}

void ParticleSystem::processParticles()
{
	while (!endParticlesProcess)
	{
		if (externalLaunchFlag)
		{
			createParticles();
			launched = true;
			externalLaunchFlag = false;
		}

		if (!updateStatus && launched)
		{
			updateMut.lock();
			updateStatus = true;

			int deadParticles = 0;
			for (int i = 0; i < particlesCount; ++i)
			{
				particles[i].update(Time::deltaTime);
				if (particles[i].lifeTime <= 0.0f)
				{
					++deadParticles;
					if (continuous)
					{
						setParticleRandomData(particles[i]);
					}
				}

				particles[i].distanceFromCamera = glm::length2(particles[i].pos - camera->getTransform().getPosition());
			}

			std::sort(particles.begin(), particles.end());
			updateRendererBuffers();

			if (deadParticles == particlesCount && !continuous)
			{
				launched = false;
			}
			updateMut.unlock();
		}
	}
}

void ParticleSystem::invalidate()
{
	endParticlesProcess = true;
	//if (isPauseMutexLocked) run();
	particleThread.join();
}

void ParticleSystem::setActive(bool val)
{
	GameObject::setActive(val);

	//if (val) run();
	//else pause();

	renderer->setActive(val);
}

//void ParticleSystem::pause()
//{
//	if (!isPauseMutexLocked)
//	{
//		pauseMutex.lock();
//		isPauseMutexLocked = true;
//	}
//}
//
//void ParticleSystem::run()
//{
//	if (isPauseMutexLocked)
//	{
//		pauseMutex.unlock();
//		isPauseMutexLocked = false;
//	}
//}

void ParticleSystem::launch()
{
	//createParticles();
	externalLaunchFlag = true;
	//pauseMutex.lock();
	//launched = true;
	//pauseMutex.unlock();
	//run();
}

ParticleRendererPtr ParticleSystem::getRenderer()const
{
	return renderer;
}

void ParticleSystem::registerCamera(CameraPtr camera)
{
	this->camera = camera;
}

void ParticleSystem::setParticlesCount(int count)
{
	particlesCount = count;
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
	particleSize = val;
}

void ParticleSystem::setParticlesSpreadCone(float minAngle, float maxAngle)
{
	spreadCone.x = minAngle;
	spreadCone.y = maxAngle;
}

void ParticleSystem::setColors(const glm::vec3& base, const glm::vec3& dest)
{
	baseColor = base;
	destColor = dest;
}

void ParticleSystem::setBlendingFunctions(GLenum sfactor, GLenum dfactor)
{
	blendFunc.sfactor = sfactor;
	blendFunc.dfactor = dfactor;
}

void ParticleSystem::setEvenSpread()
{
	evenSpred = true;
}

void ParticleSystem::setSingleSpread()
{
	continuous = false;
}

void ParticleSystem::setContinuousSpred()
{
	continuous = true;
}

int ParticleSystem::getParticlesCount()const
{
	return particlesCount;
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
	return particleSize;
}

glm::vec2 ParticleSystem::getParticlesSpreadCone()const
{
	return spreadCone;
}

glm::vec3 ParticleSystem::getBaseColor()const
{
	return baseColor;
}

glm::vec3 ParticleSystem::getDestColor()const
{
	return destColor;
}

bool ParticleSystem::isRunning()const
{
	return (!endParticlesProcess && launched);
}

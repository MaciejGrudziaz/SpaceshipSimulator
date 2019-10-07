#include "particlesystem.h"

#ifdef OLD_VERSION

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
	, particlesUpdated(0)
	, deadParticles(0)
	, particlesUpdateCountPerRefresh(3000)
{}

ParticleSystem::~ParticleSystem()
{
	for(auto particle : particles)
	{
		delete particle;
	}
}

void ParticleSystem::loadRenderer(const ParticleSystemFiles& data, const ModelExternalUniforms& uniforms)
{
	renderer = std::make_shared<ParticleRenderer>(particlesPosSizeBuffer, particlesColorBuffer, blendFunc);
	renderer->setParticlesCount(particlesCount);
	renderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	initUniforms(uniforms);
	renderer->loadTexture(data.particleTexture);
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

void ParticleSystem::createParticles()
{
	particles.clear();
	for (int i = 0; i < particlesCount; ++i)
	{
		Particle* particle = new Particle();
		//setParticleRandomData(particle);
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

	//particle.pos = modelTransform * glm::vec4(0.0f,0.0f,0.0f,1.0f);
	glm::vec3 newPos = modelTransform * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	*(particle.pos[0]) = newPos.x;
	*(particle.pos[1]) = newPos.y;
	*(particle.pos[2]) = newPos.z;
	*(particle.size) = randSize(rng);
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
	//else {
	//	glm::vec3 color;
	//	float lifetimeFrac;
	//	for(int i = 0;i<particlesCount; ++i)
	//	{
	//		memcpy(particlesPosSizeBuffer.data() + bufferVertexAttribSize * i, glm::value_ptr(particles[i].pos), 3 * sizeof(float));
	//		particlesPosSizeBuffer[bufferVertexAttribSize * i + 3] = particles[i].size;

	//		lifetimeFrac = particles[i].lifeTime / particles[i].maxLifeTime;
	//		color = baseColor;
	//		color += (destColor - baseColor) * (1.0f - lifetimeFrac);
	//		
	//		memcpy(particlesColorBuffer.data() + bufferVertexAttribSize * i, glm::value_ptr(color), 3 * sizeof(float));
	//		particlesColorBuffer[bufferVertexAttribSize * i + 3] = lifetimeFrac;

	//		if (renderer)
	//		{
	//			renderer->updatePositionBuffer();
	//			renderer->updateColorBuffer();
	//		}
	//	}
	//}
}

void ParticleSystem::createRendererBuffers()
{
	if (particlesPosSizeBuffer.size() == 0)
	{
		int i = 0;
		particlesPosSizeBuffer = std::vector<float>(particlesCount * 4);
		//for (int i = 0; i < particlesCount; ++i)
		for(auto particle : particles)
		{
			//std::memcpy(particlesPosSizeBuffer.data() + (i * 4), glm::value_ptr(particles[i].pos), 3 * sizeof(float));
			//std::memcpy(particlesPosSizeBuffer.data() + (i * 4 + 3), &particles[i].size, sizeof(float));
			float* buffer = particlesPosSizeBuffer.data();
			particle->linkPosWithParticlesBuffer(buffer + (4 * i), buffer + (4 * i + 1), buffer + (4 * i + 2));
			particle->linkSizeWithParticlesBuffer(buffer + (4 * i + 3));
			++i;
		}
	}
	if (particlesColorBuffer.size() == 0)
	{
		particlesColorBuffer = std::vector<float>(particlesCount * 4);
		glm::vec3 color;
		float lifetimeFrac;

		int i = 0;
		for (auto particle : particles)
		{
			//lifetimeFrac = particles[i].lifeTime / particles[i].maxLifeTime;
			//color = baseColor;
			//color += (destColor - baseColor) * (1.0f - lifetimeFrac);
			//std::memcpy(particlesColorBuffer.data() + (i * 4), glm::value_ptr(particles[i].pos), 3 * sizeof(float));
			//std::memcpy(particlesColorBuffer.data() + (i * 4 + 3), &particles[i].size, sizeof(float));

			float* buffer = particlesColorBuffer.data();
			particle->linkColorWithParticlesBuffer(buffer + (4 * i), buffer + (4 * i + 1), buffer + (4 * i + 2), buffer + (4 * i + 3));
			++i;
		}
	}
}

void ParticleSystem::init()
{
	GameObject::init();

	createParticles();
	createRendererBuffers();

	if(renderer)
		renderer->init();

	startThread();
}

void ParticleSystem::startThread()
{
	updateStatus = true;
	particleThread = std::thread(&ParticleSystem::processParticles, this);
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
			recreateParticles();
			launched = true;
			externalLaunchFlag = false;
		}

		if (!updateStatus && launched)
		{
			updateMut.lock();
			updateStatus = true;

			int deadParticles = 0;
			int i = 0;
			//for (int i = 0; i < particlesCount; ++i)
			for(auto particle : particles)
			{
				particle->update(Time::deltaTime, baseColor, destColor);
				if (particle->lifeTime <= 0.0f)
				{
					++deadParticles;
					if (continuous)
					{
						setParticleRandomData(*particle);
					}
				}
				particle->calcDistance(camera->getTransform().getPosition());
				++i;
			}

			//std::sort(particles.begin(), particles.end());

			//if (particlesUpdated == 0) deadParticles = 0;
			//int updateCount = particlesUpdateCountPerRefresh;
			//if (particlesUpdated + updateCount > particlesCount)
			//	updateCount = particlesCount - particlesUpdated;

			//for (int i = particlesUpdated; i < particlesUpdated + updateCount; ++i)
			//{
			//	particles[i]->update(Time::deltaTime * (1.0f/particlesUpdatedPerFrameFrac), baseColor, destColor);
			//	if (particles[i]->lifeTime <= 0.0f)
			//	{
			//		++deadParticles;
			//		if (continuous)
			//		{
			//			setParticleRandomData(*particles[i]);
			//		}
			//	}
			//	particles[i]->calcDistance(camera->getTransform().getPosition());
			//}

			//particlesUpdated += updateCount;
			//if (particlesUpdated == particlesCount)
			//	particlesUpdated = 0;

			if (renderer)
			{
				renderer->updatePositionBuffer();
				renderer->updateColorBuffer();
			}

			if (deadParticles == particlesCount && !continuous)
			{
				launched = false;
			}
			updateMut.unlock();
		}
	}
}

void ParticleSystem::recreateParticles()
{
	for (auto particle : particles)
	{
		setParticleRandomData(*particle);
	}
}

void ParticleSystem::invalidate()
{
	GameObject::invalidate();

	if (renderer)
		renderer->invalidate();

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

	particlesUpdatedPerFrameFrac = particlesUpdateCountPerRefresh / particlesCount;

	if (particlesUpdatedPerFrameFrac > 1.0f) particlesUpdatedPerFrameFrac = 1.0f;
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

void ParticleSystem::setParticlesUpdateCountPerRefresh(int val)
{
	if (!launched)
	{
		particlesUpdateCountPerRefresh = val;
		particlesUpdatedPerFrameFrac = static_cast<float>(particlesUpdateCountPerRefresh) / static_cast<float>(particlesCount);

		if (particlesUpdatedPerFrameFrac > 1.0f) particlesUpdatedPerFrameFrac = 1.0f;
	}
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

BlendFunctions ParticleSystem::getBlendFunctions()const
{
	return blendFunc;
}

bool ParticleSystem::isRunning()const
{
	return (!endParticlesProcess && (externalLaunchFlag || launched));
}

std::vector<float>& ParticleSystem::getParticlesPosSizeBuffer()
{
	return particlesPosSizeBuffer;
}

std::vector<float>& ParticleSystem::getParticlesColorBuffer()
{
	return particlesColorBuffer;
}

#endif

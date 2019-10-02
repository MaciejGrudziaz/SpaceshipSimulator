#include "particlesystem.h"

ParticleSystem::ParticleSystem()
	: particlesCount(0)
	, particleSize(1.0f)
	, spreadCone(-45.0f, 45.0f)
	, maxSpeed(0.0f)
	, maxLifetime(0.0f)
	, bufferVertexAttribSize(4)
	, baseColor(1.0f,1.0f,1.0f)
	, destColor(1.0f,1.0f,1.0f)
	, calc(true)
{}

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

void ParticleSystem::load(const ParticleSystemData& data, const ModelExternalUniforms& uniforms)
{
	createParticles();

	renderer = std::make_shared<ParticleRenderer>(particlesPosSizeBuffer, particlesColorBuffer);
	renderer->setParticlesCount(particlesCount);
	renderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	initUniforms(uniforms);
	renderer->init();
	renderer->loadBuffers();
	renderer->loadTexture(data.particleTexture);

	updateRendererBuffers();

	updateStatus = { false, false, false, false, false };

	//calcParticlesThreads[0] = std::thread(&ParticleSystem::processParticlesPart1, this);
	//calcParticlesThreads[1] = std::thread(&ParticleSystem::processParticlesPart2, this);
	//calcParticlesThreads[2] = std::thread(&ParticleSystem::processParticlesPart3, this);
	//calcParticlesThreads[3] = std::thread(&ParticleSystem::processParticlesPart4, this);
	//calcParticlesThreads[4] = std::thread(&ParticleSystem::sortParticles, this);
	particleThread = std::thread(&ParticleSystem::sortParticles, this);
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
	std::uniform_real_distribution<> randLifetime(0.0f, maxLifetime);
	std::uniform_real_distribution<> randSpeed(0.8 * maxSpeed, 1.2 * maxSpeed);
	std::uniform_real_distribution<> randRotAngle(spreadCone.x, spreadCone.y);
	std::uniform_real_distribution<> randAxis(0.0f, 1.0f);
	std::uniform_real_distribution<> fullRandAxis(-1.0f, 1.0f);
	std::uniform_real_distribution<> randSize(0.5f * particleSize, 1.2 * particleSize);

	particle.pos = transform.getPosition();
	particle.size = randSize(rng);
	particle.maxLifeTime = maxLifetime;
	particle.lifeTime = randLifetime(rng);
	particle.distanceFromCamera = 0.0f;
	//glm::vec3 transformedDir = transform.getRotationQuat() * directionVec;
	glm::vec3 directionVec = transform.getOrientation();

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

	processParticles();
}

void ParticleSystem::processParticles()
{
	//for (int i = 0; i < particlesCount; ++i)
	//{
	//	particles[i].update(Time::deltaTime);
	//	particles[i].distanceFromCamera = glm::length2(particles[i].pos - camera->getTransform().getPosition());

	//	if (particles[i].lifeTime <= 0.0f)
	//		setParticleRandomData(particles[i]);
	//}
	//std::thread th1(&ParticleSystem::processParticlesPart1, this);
	//std::thread th2(&ParticleSystem::processParticlesPart2, this);
	//std::thread th3(&ParticleSystem::processParticlesPart3, this);
	//std::thread th4(&ParticleSystem::processParticlesPart4, this);
	//th1.join();
	//th2.join();
	//th3.join();
	//th4.join();

	if (updateStatus[4])
	{
		updateMut.lock();
		//part1Update.lock();
		//part2Update.lock();
		//part3Update.lock();
		//part4Update.lock();

		
		//updateStatus = { false, false, false, false, false };
		updateStatus[4] = false;
		updateMut.unlock();

		//part1Update.unlock();
		//part2Update.unlock();
		//part3Update.unlock();
		//part4Update.unlock();
	}
}

void ParticleSystem::processParticlesPart1()
{
	int beginIdx = 0, endIdx = particlesCount / 4;

	//while (calc)
	//{
	//	if (!updateStatus[0])
	//	{
	//		part1Update.lock();
	//		updateStatus[0] = true;
			for (int i = beginIdx; i < endIdx; ++i)
			{
				particles[i].update(Time::deltaTime);
				particles[i].distanceFromCamera = glm::length2(particles[i].pos - camera->getTransform().getPosition());

				if (particles[i].lifeTime <= 0.0f)
					setParticleRandomData(particles[i]);
			}
	//		part1Update.unlock();
	//	}
	//}
}

void ParticleSystem::processParticlesPart2()
{
	int beginIdx = particlesCount / 4, endIdx = 2 * particlesCount / 4;

	//while (calc)
	//{
	//	if (!updateStatus[1])
	//	{
			//part2Update.lock();
			//updateStatus[1] = true;
			for (int i = beginIdx; i < endIdx; ++i)
			{
				particles[i].update(Time::deltaTime);
				particles[i].distanceFromCamera = glm::length2(particles[i].pos - camera->getTransform().getPosition());

				if (particles[i].lifeTime <= 0.0f)
					setParticleRandomData(particles[i]);
			}
	//		part2Update.unlock();
	//	}
	//}
}

void ParticleSystem::processParticlesPart3()
{
	int beginIdx = 2 * particlesCount / 4, endIdx = 3 * particlesCount / 4;

	//while (calc)
	//{
	//	if (!updateStatus[2])
	//	{
			//part3Update.lock();
			//updateStatus[2] = true;
			for (int i = beginIdx; i < endIdx; ++i)
			{
				particles[i].update(Time::deltaTime);
				particles[i].distanceFromCamera = glm::length2(particles[i].pos - camera->getTransform().getPosition());

				if (particles[i].lifeTime <= 0.0f)
					setParticleRandomData(particles[i]);
			}
	//		part3Update.unlock();
	//	}
	//}
}

void ParticleSystem::processParticlesPart4()
{
	int beginIdx = 3 * particlesCount / 4, endIdx = particlesCount;

	//while (calc)
	//{
		//if (!updateStatus[3])
		//{
		//	part4Update.lock();
		//	updateStatus[3] = true;
			for (int i = beginIdx; i < endIdx; ++i)
			{
				particles[i].update(Time::deltaTime);
				particles[i].distanceFromCamera = glm::length2(particles[i].pos - camera->getTransform().getPosition());

				if (particles[i].lifeTime <= 0.0f)
					setParticleRandomData(particles[i]);
			}
		//	part4Update.unlock();
		//}
	//}
}

void ParticleSystem::sortParticles()
{
	while (calc)
	{
		if (!updateStatus[4])
		{
			//if (updateStatus[0] && updateStatus[1] && updateStatus[2] && updateStatus[3])
			//{
				//part1Update.lock();
				//part2Update.lock();
				//part3Update.lock();
				//part4Update.lock();

				updateMut.lock();
				updateStatus[4] = true;

				processParticlesPart1();
				processParticlesPart2();
				processParticlesPart3();
				processParticlesPart4();
				
				std::sort(particles.begin(), particles.end());
				updateRendererBuffers();

				updateMut.unlock();

				//part1Update.unlock();
				//part2Update.unlock();
				//part3Update.unlock();
				//part4Update.unlock();
			//}
		}
	}
}

void ParticleSystem::invalidate()
{
	calc = false;
	//for (int i = 0; i < 5; ++i)
	//	calcParticlesThreads[i].join();
	particleThread.join();
}

ParticleRendererPtr ParticleSystem::getRenderer()const
{
	return renderer;
}

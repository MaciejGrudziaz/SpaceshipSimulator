#include "particlesystemv2.h"

ParticleSystemV2::ParticleSystemV2()
	: modelTransform(std::make_shared<glm::mat4>(1.0f))
	, particlesCount(0)
	, particleSize(1.0f)
	, spreadCone(-45.0f, 45.0f)
	, maxSpeed(0.0f)
	, maxLifetime(0.0f)
	, baseColor(std::make_shared<glm::vec3>(1.0f))
	, destColor(std::make_shared<glm::vec3>(1.0f))
	//, endParticlesProcess(false)
	, evenSpred(false)
	, continuous(std::make_shared<int>(1))
	, runTime(std::make_shared<float>(0.0f))
	, shutDownTime(std::make_shared<float>(0.0f))
	//, launched(false)
	, launchFlag(false)
	, shutingDownFlag(false)
	//, particlesUpdated(0)
	//, deadParticles(0)
	//, particlesUpdateCountPerRefresh(3000)
{}

ParticleSystemV2::~ParticleSystemV2()
{}

void ParticleSystemV2::loadRenderer(const ParticleSystemFiles& data, const ModelExternalUniforms& uniforms)
{
	renderer = std::make_shared<ParticleRendererV2>(particlesBuffer, blendFunc);
	renderer->setActive(false);
	renderer->setParticlesCount(particlesCount);
	renderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	initUniforms(uniforms);
	renderer->loadTexture(data.particleTexture);
}

void ParticleSystemV2::initUniforms(const ModelExternalUniforms& uniforms)
{
	UniformDataMat4Ptr modelUniform = std::make_shared<UniformDataMat4>("model");
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");
	UniformDataVec3Ptr cameraUpUniform = std::make_shared<UniformDataVec3>("cameraUp");
	UniformDataVec3Ptr cameraRightUniform = std::make_shared<UniformDataVec3>("cameraRight");
	UniformDataFloatPtr shutDownTimeUniform = std::make_shared<UniformDataFloat>("shutDownTime");
	UniformDataIntPtr continuousUniform = std::make_shared<UniformDataInt>("continuous");
	UniformDataFloatPtr runTimeUniform = std::make_shared<UniformDataFloat>("runTime");
	UniformDataVec3Ptr baseColorUniform = std::make_shared<UniformDataVec3>("baseColor");
	UniformDataVec3Ptr destColorUniform = std::make_shared<UniformDataVec3>("destColor");

	modelUniform->mat = modelTransform;
	viewUniform->mat = uniforms.view;
	projectionUniform->mat = uniforms.projection;
	cameraUpUniform->vec = camera->getUpVecPtr();
	cameraRightUniform->vec = camera->getRightVecPtr();
	shutDownTimeUniform->val = shutDownTime;
	continuousUniform->val = continuous;
	runTimeUniform->val = runTime;
	baseColorUniform->vec = baseColor;
	destColorUniform->vec = destColor;

	renderer->addUniform(modelUniform);
	renderer->addUniform(viewUniform);
	renderer->addUniform(projectionUniform);
	renderer->addUniform(cameraUpUniform);
	renderer->addUniform(cameraRightUniform);
	renderer->addUniform(shutDownTimeUniform);
	renderer->addUniform(continuousUniform);
	renderer->addUniform(runTimeUniform);
	renderer->addUniform(baseColorUniform);
	renderer->addUniform(destColorUniform);
}

void ParticleSystemV2::createParticles()
{
	particles.clear();
	for (int i = 0; i < particlesCount; ++i)
	{
		ParticleV2Ptr particle = std::make_shared<ParticleV2>();
		setParticleRandomData(*particle);
		particles.push_back(particle);
	}
}

void ParticleSystemV2::setParticleRandomData(ParticleV2& particle)
{
	std::uniform_real_distribution<> randLifetime(0.0f, maxLifetime);
	std::uniform_real_distribution<> randSpeed(0.2 * maxSpeed, 1.2 * maxSpeed);
	std::uniform_real_distribution<> randRotAngle(spreadCone.x, spreadCone.y);
	std::uniform_real_distribution<> fullRandAxis(-1.0f, 1.0f);
	std::uniform_real_distribution<> randSize(0.5f * particleSize, 1.2 * particleSize);

	particle.pos = (*modelTransform) * glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
	particle.size = randSize(rng);
	particle.lifeTime = randLifetime(rng);

	if (!evenSpred)
	{
		glm::vec3 directionVec = parentTransform * glm::vec4(transform.getOrientation(), 0.0f);
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

glm::vec3 ParticleSystemV2::getNormalVec(const glm::vec3& vec)
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

void ParticleSystemV2::updateRendererBuffers()
{
	//if (particlesBuffer.size() == 0)
	//{
	//	createRendererBuffers();
	//}
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

void ParticleSystemV2::createRendererBuffers()
{
	if (particlesBuffer.size() == 0)
	{
		particlesBuffer = std::vector<float>(particlesCount * bufferVertexAttribSize);
		//for (int i = 0; i < particlesCount; ++i)
		int i = 0;
		for (auto particle : particles)
		{
			std::memcpy(particlesBuffer.data() + (i * bufferVertexAttribSize), glm::value_ptr(particle->speed), 3 * sizeof(float));
			std::memcpy(particlesBuffer.data() + (i * bufferVertexAttribSize + 3), glm::value_ptr(particle->pos), sizeof(float));
			std::memcpy(particlesBuffer.data() + (i * bufferVertexAttribSize + 6), &particle->size, sizeof(float));
			std::memcpy(particlesBuffer.data() + (i * bufferVertexAttribSize + 7), &particle->lifeTime, sizeof(float));
			++i;
		}
	}
	//if (particlesColorBuffer.size() == 0)
	//{
	//	particlesColorBuffer = std::vector<float>(particlesCount * 4);
	//	glm::vec3 color;
	//	float lifetimeFrac;

	//	int i = 0;
	//	for (auto particle : particles)
	//	{
	//		//lifetimeFrac = particles[i].lifeTime / particles[i].maxLifeTime;
	//		//color = baseColor;
	//		//color += (destColor - baseColor) * (1.0f - lifetimeFrac);
	//		//std::memcpy(particlesColorBuffer.data() + (i * 4), glm::value_ptr(particles[i].pos), 3 * sizeof(float));
	//		//std::memcpy(particlesColorBuffer.data() + (i * 4 + 3), &particles[i].size, sizeof(float));

	//		float* buffer = particlesColorBuffer.data();
	//		particle->linkColorWithParticlesBuffer(buffer + (4 * i), buffer + (4 * i + 1), buffer + (4 * i + 2), buffer + (4 * i + 3));
	//		++i;
	//	}
	//}
}

void ParticleSystemV2::init()
{
	GameObject::init();

	createParticles();
	createRendererBuffers();
	if (renderer)
	{
		renderer->updateBuffer();
		renderer->init();
	}
}

void ParticleSystemV2::process()
{
	GameObject::process();

	(*modelTransform) = parentTransform * getTransform().getTransformMat();

	if (launchFlag)
	{
		*runTime += Time::deltaTime;

		if (*continuous == 0)
		{
			if ((*runTime - *shutDownTime) > maxLifetime)
			{
				launchFlag = false;
				shutingDownFlag = false;
				*runTime = 0.0f;
				*shutDownTime = 0.0f;
				renderer->setActive(false);
			}
		}
	}

}

void ParticleSystemV2::recreateParticles()
{
	for (auto particle : particles)
	{
		setParticleRandomData(*particle);
	}
}

void ParticleSystemV2::invalidate()
{
	GameObject::invalidate();

	if (renderer)
		renderer->invalidate();
}

void ParticleSystemV2::setActive(bool val)
{
	GameObject::setActive(val);

	if (renderer)
		renderer->setActive(val);
}

void ParticleSystemV2::launch()
{
	launchFlag = true;
	shutingDownFlag = false;
	*runTime = 0.0f;
	*shutDownTime = 0.0f;

	if (renderer)
		renderer->setActive(true);
}

ParticleRendererV2Ptr ParticleSystemV2::getRenderer()const
{
	return renderer;
}

void ParticleSystemV2::registerCamera(CameraPtr camera)
{
	this->camera = camera;
}

void ParticleSystemV2::setParticlesCount(int count)
{
	particlesCount = count;
}

void ParticleSystemV2::setParticlesMaxSpeed(float val)
{
	maxSpeed = val;
}

void ParticleSystemV2::setParticlesMaxLifetime(float val)
{
	maxLifetime = val;
}

void ParticleSystemV2::setParticlesSize(float val)
{
	particleSize = val;
}

void ParticleSystemV2::setParticlesSpreadCone(float minAngle, float maxAngle)
{
	spreadCone.x = minAngle;
	spreadCone.y = maxAngle;
}

void ParticleSystemV2::setColors(const glm::vec3& base, const glm::vec3& dest)
{
	*baseColor = base;
	*destColor = dest;
}

void ParticleSystemV2::setBlendingFunctions(GLenum sfactor, GLenum dfactor)
{
	blendFunc.sfactor = sfactor;
	blendFunc.dfactor = dfactor;
}

void ParticleSystemV2::setEvenSpread()
{
	evenSpred = true;
}

void ParticleSystemV2::setSingleSpread()
{
	//if (!shutingDownFlag)
	//{
	//	*continuous = 0;
	//	*shutDownTime = *runTime;
	//	shutingDownFlag = true;
	//}
	*continuous = 0;
}

float ParticleSystemV2::shutDown()
{
	if (!shutingDownFlag)
	{
		*shutDownTime = *runTime;
		shutingDownFlag = true;
	}

	return *shutDownTime;
}

void ParticleSystemV2::setContinuousSpred()
{
	*continuous = 1;
}

int ParticleSystemV2::getParticlesCount()const
{
	return particlesCount;
}

float ParticleSystemV2::getParticlesMaxSpeed()const
{
	return maxSpeed;
}

float ParticleSystemV2::getParticlesMaxLifetime()const
{
	return maxLifetime;
}

float ParticleSystemV2::getParticlesSize()const
{
	return particleSize;
}

glm::vec2 ParticleSystemV2::getParticlesSpreadCone()const
{
	return spreadCone;
}

glm::vec3 ParticleSystemV2::getBaseColor()const
{
	return *baseColor;
}

glm::vec3 ParticleSystemV2::getDestColor()const
{
	return *destColor;
}

BlendFunctions ParticleSystemV2::getBlendFunctions()const
{
	return blendFunc;
}

bool ParticleSystemV2::isRunning()const
{
	return (launchFlag && !shutingDownFlag);
}

std::vector<float>& ParticleSystemV2::getParticlesBuffer()
{
	return particlesBuffer;
}

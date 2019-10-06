#include "asteroidsmanager.h"

AsteroidsManager::AsteroidsManager()
	: renderer(std::make_shared<MultipleAsteroidsRenderer>())
	//, particlesSystem(std::make_shared<MultiSourceParticleSystem>())
{}

void AsteroidsManager::create(int initialCount, CameraPtr camera, ConstMat4Ptr projectionPtr)
{
	this->camera = camera;
	//this->particlesSystem->registerCamera(camera);
	this->projectionPtr = projectionPtr;
	createPatternAsteroid();
	createPatternAsteroidHitbox();
	createPatternAsteroidExplosionParticleSystem();

	initializeParticlesSystem();
	loadParticlesSystemRenderer();
	initializeAsteroidsVector(initialCount);
	//renderer->registerRenderer(particlesSystem->getRenderer());
}

void AsteroidsManager::createPatternAsteroid()
{
	ModelData filesData;
	filesData.modelFilename = "models/asteroid_1.mgr";
	filesData.vertexShaderFilename = "shaders/spaceshipShader.vert";
	filesData.fragmentShaderFilename = "shaders/spaceshipShader.frag";
	filesData.textureFilename = "models/textures/asteroid_1_texture.png";

	ModelExternalUniforms uniforms;
	uniforms.view = camera->getViewPtr();
	uniforms.projection = projectionPtr;

	AsteroidPtr asteroid = std::make_shared<Asteroid>();
	asteroid->load(filesData, uniforms);
	asteroid->setName("asteroid");
	asteroid->getTransform().setScale(glm::vec3(0.75f, 0.75f, 0.75f));
	asteroid->registerCamera(camera);
	asteroid->registerProjectionMatrixPtr(projectionPtr);

	patternAsteroid = asteroid;
}

void AsteroidsManager::createPatternAsteroidHitbox()
{
	HitboxObjectPtr asteroidHitbox = std::make_shared<HitboxObject>();
	asteroidHitbox->setName("hitbox_main");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/asteroidHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = projectionPtr;
	uniforms.view = camera->getViewPtr();

	asteroidHitbox->load(data, uniforms);

	asteroidHitboxPattern = asteroidHitbox;
}

void AsteroidsManager::createPatternAsteroidExplosionParticleSystem()
{
	//asteroidsExplosionPattern = std::make_shared<ParticleSystem>();
	//asteroidsExplosionPattern->setParticlesCount(4000);
	//asteroidsExplosionPattern->setParticlesMaxSpeed(6.5f);
	//asteroidsExplosionPattern->setParticlesMaxLifetime(1.0f);
	//asteroidsExplosionPattern->setParticlesSize(0.5f);
	//asteroidsExplosionPattern->setEvenSpread();
	//asteroidsExplosionPattern->setColors(glm::vec3(1.0f, 0.55f, 0.1f), glm::vec3(0.7f, 0.7f, 0.7f));
	//asteroidsExplosionPattern->registerCamera(camera);
	//asteroidsExplosionPattern->getTransform().setPosition(glm::vec3(0.0f));
	//asteroidsExplosionPattern->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE);

	//ParticleSystemData data;
	//data.particleTexture = "sprites/asteroid_particle.png";
	//data.vertexShaderFilename = "shaders/particle.vert";
	//data.fragmentShaderFilename = "shaders/particle.frag";

	//ModelExternalUniforms uniforms;
	//uniforms.view = camera->getViewPtr();
	//uniforms.projection = projectionPtr;

	//asteroidsExplosionPattern->load(data, uniforms);

	//renderer->registerRenderer(asteroidsExplosionPattern->getRenderer());

	//asteroidsExplosionPattern->setSingleSpread();
	//asteroidsExplosionPattern->launch();

	//asteroidsExplosionFragmentsPattern = std::make_shared<ParticleSystem>();
	//asteroidsExplosionFragmentsPattern->setParticlesCount(2000);
	//asteroidsExplosionFragmentsPattern->setParticlesMaxSpeed(5.0f);
	//asteroidsExplosionFragmentsPattern->setParticlesMaxLifetime(3.0f);
	//asteroidsExplosionFragmentsPattern->setParticlesSize(0.6f);
	//asteroidsExplosionFragmentsPattern->setEvenSpread();
	//asteroidsExplosionFragmentsPattern->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.7f, 0.7f, 0.7f));
	//asteroidsExplosionFragmentsPattern->registerCamera(camera);
	//asteroidsExplosionFragmentsPattern->getTransform().setPosition(glm::vec3(0.0f));
	//asteroidsExplosionFragmentsPattern->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE);

	//asteroidsExplosionFragmentsPattern->load(data, uniforms);

	//renderer->registerRenderer(asteroidsExplosionFragmentsPattern->getRenderer());

	//asteroidsExplosionFragmentsPattern->setSingleSpread();
	//asteroidsExplosionFragmentsPattern->launch();
}

void AsteroidsManager::initializeAsteroidsVector(int initialCount)
{
	findLimitPosY();
	findLimitPosX();

	for (int i = 0; i < initialCount; ++i)
	{
		AsteroidPtr asteroid = std::make_shared<Asteroid>();
		asteroid->deepCopy(*patternAsteroid);

		HitboxObjectPtr hitbox = std::make_shared<HitboxObject>();
		hitbox->deepCopy(*asteroidHitboxPattern);
		hitbox->setName("hitbox_main");

		asteroid->addChild(hitbox);

		asteroid->registerWorldSpeed(worldSpeed);

		initializeAsteroid(asteroid);
		//asteroid->initParticleSystems();

		asteroid->initParticleSystemsData(explosionParticles->getParticlesMaxLifetime(), explosionFragmentsParticles->getParticlesMaxLifetime());
		explosionParticlesRenderer->registerParticleSystemData(asteroid->getExplosionParticleSystemData());
		explosionFragmentsParticlesRenderer->registerParticleSystemData(asteroid->getExplosionFragmentsParticleSystemData());

		renderer->registerRenderer(asteroid->getRenderer());
		//renderer->registerRenderer(asteroid->getExplosionParticlesRenderer());
		//renderer->registerRenderer(asteroid->getExplosionFragmentsParticlesRenderer());
		renderer->registerRenderer(hitbox->getRenderer());

		asteroids.push_back(asteroid);
	}
}

void AsteroidsManager::initializeAsteroid(AsteroidPtr asteroid)
{
	std::uniform_real_distribution<> linearSpeedRandX(0.0f, 3.0f);
	std::uniform_real_distribution<> linearSpeedRandY(-10.0f, -4.0f);
	std::uniform_real_distribution<> rotSpeedRand(-5.0f, 5.0f);
	std::uniform_real_distribution<> xPosRand(0.8f * arenaLimits.minX, 0.8f * arenaLimits.maxX);
	std::uniform_real_distribution<> sizeRand(0.5f, 1.2f);

	float spawnPosY = 1.2f * arenaLimits.maxY;
	glm::vec3 spawnPos(0.0f, spawnPosY, 0.0f);
	glm::vec3 spawnRotSpeed;
	glm::vec3 spawnLinSpeed;
	float spawnSize;

	spawnPos.x = xPosRand(rng);

	if (spawnPos.x < 0.0f) spawnLinSpeed.x = (spawnPos.x / arenaLimits.minX) * linearSpeedRandX(rng);
	else spawnLinSpeed.x = -(spawnPos.x / arenaLimits.maxX) * linearSpeedRandX(rng);

	spawnLinSpeed.y = linearSpeedRandY(rng);

	spawnRotSpeed.x = rotSpeedRand(rng);
	spawnRotSpeed.y = rotSpeedRand(rng);
	spawnRotSpeed.z = rotSpeedRand(rng);

	spawnSize = sizeRand(rng);

	asteroid->getTransform().setPosition(spawnPos);
	asteroid->getTransform().setScale(glm::vec3(spawnSize, spawnSize, spawnSize));
	asteroid->setLinearSpeed(spawnLinSpeed);
	asteroid->setRotSpeed(spawnRotSpeed);
	asteroid->setHealth(100.0f * spawnSize);
}

void AsteroidsManager::loadParticlesSystemRenderer()
{
	ParticleSystemFiles data;
	data.particleTexture = "sprites/asteroid_particle.png";
	data.vertexShaderFilename = "shaders/particleV2.vert";
	data.fragmentShaderFilename = "shaders/particle.frag";

	explosionParticlesRenderer = std::make_shared<MultiSourceParticleRenderer>(explosionParticles->getParticlesBuffer(), 
		explosionParticles->getBlendFunctions());
	explosionParticlesRenderer->setActive(false);
	explosionParticlesRenderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	explosionParticlesRenderer->loadTexture(data.particleTexture);
	explosionParticlesRenderer->setActive(true);
	explosionParticlesRenderer->registerUniformsPointers(explosionParticlesUniformData);
	explosionParticlesRenderer->setParticlesCount(explosionParticles->getParticlesCount());

	explosionFragmentsParticlesRenderer = std::make_shared<MultiSourceParticleRenderer>(explosionFragmentsParticles->getParticlesBuffer(),
		explosionFragmentsParticles->getBlendFunctions());
	explosionFragmentsParticlesRenderer->setActive(false);
	explosionFragmentsParticlesRenderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	explosionFragmentsParticlesRenderer->loadTexture(data.particleTexture);
	explosionFragmentsParticlesRenderer->setActive(true);
	explosionFragmentsParticlesRenderer->registerUniformsPointers(explosionFragmentsParticlesUniformData);
	explosionFragmentsParticlesRenderer->setParticlesCount(explosionFragmentsParticles->getParticlesCount());

	initUniforms();
}

void AsteroidsManager::initUniforms()
{
	initExplosionParticlesUniform();
	initExplosionFragmentParticlesUniform();
}

void AsteroidsManager::initExplosionParticlesUniform()
{
	explosionParticlesUniformData = std::make_shared<ParticleSystemData>();
	explosionParticlesUniformData->init();
	explosionParticlesRenderer->registerUniformsPointers(explosionParticlesUniformData);

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

	modelUniform->mat = explosionParticlesUniformData->modelTransform;
	viewUniform->mat = camera->getViewPtr();
	projectionUniform->mat = projectionPtr;
	cameraUpUniform->vec = camera->getUpVecPtr();
	cameraRightUniform->vec = camera->getRightVecPtr();
	shutDownTimeUniform->val = explosionParticlesUniformData->shutDownTime;
	continuousUniform->val = explosionParticlesUniformData->continuous;
	runTimeUniform->val = explosionParticlesUniformData->runTime;
	baseColorUniform->vec = explosionParticlesUniformData->baseColor;
	destColorUniform->vec = explosionParticlesUniformData->destColor;

	explosionParticlesRenderer->addUniform(modelUniform);
	explosionParticlesRenderer->addUniform(viewUniform);
	explosionParticlesRenderer->addUniform(projectionUniform);
	explosionParticlesRenderer->addUniform(cameraUpUniform);
	explosionParticlesRenderer->addUniform(cameraRightUniform);
	explosionParticlesRenderer->addUniform(shutDownTimeUniform);
	explosionParticlesRenderer->addUniform(continuousUniform);
	explosionParticlesRenderer->addUniform(runTimeUniform);
	explosionParticlesRenderer->addUniform(baseColorUniform);
	explosionParticlesRenderer->addUniform(destColorUniform);
}

void AsteroidsManager::initExplosionFragmentParticlesUniform()
{
	explosionFragmentsParticlesUniformData = std::make_shared<ParticleSystemData>();
	explosionFragmentsParticlesUniformData->init();
	explosionFragmentsParticlesRenderer->registerUniformsPointers(explosionFragmentsParticlesUniformData);

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

	modelUniform->mat = explosionFragmentsParticlesUniformData->modelTransform;
	viewUniform->mat = camera->getViewPtr();
	projectionUniform->mat = projectionPtr;
	cameraUpUniform->vec = camera->getUpVecPtr();
	cameraRightUniform->vec = camera->getRightVecPtr();
	shutDownTimeUniform->val = explosionFragmentsParticlesUniformData->shutDownTime;
	continuousUniform->val = explosionFragmentsParticlesUniformData->continuous;
	runTimeUniform->val = explosionFragmentsParticlesUniformData->runTime;
	baseColorUniform->vec = explosionFragmentsParticlesUniformData->baseColor;
	destColorUniform->vec = explosionFragmentsParticlesUniformData->destColor;

	explosionFragmentsParticlesRenderer->addUniform(modelUniform);
	explosionFragmentsParticlesRenderer->addUniform(viewUniform);
	explosionFragmentsParticlesRenderer->addUniform(projectionUniform);
	explosionFragmentsParticlesRenderer->addUniform(cameraUpUniform);
	explosionFragmentsParticlesRenderer->addUniform(cameraRightUniform);
	explosionFragmentsParticlesRenderer->addUniform(shutDownTimeUniform);
	explosionFragmentsParticlesRenderer->addUniform(continuousUniform);
	explosionFragmentsParticlesRenderer->addUniform(runTimeUniform);
	explosionFragmentsParticlesRenderer->addUniform(baseColorUniform);
	explosionFragmentsParticlesRenderer->addUniform(destColorUniform);
}

void AsteroidsManager::init()
{
	GameObject::init();

	for (auto asteroid : asteroids)
	{
		asteroid->init();
		//renderer->registerRenderer(asteroid->getExplosionParticlesRenderer());
		//renderer->registerRenderer(asteroid->getExplosionFragmentsParticlesRenderer());
		//particlesSystem->registerParticleSystem(asteroid->getExplosionParticlesSystem());
		//particlesSystem->registerParticleSystem(asteroid->getExplosionFragmentsParticlesSystem());
	}
	explosionParticles->init();
	explosionFragmentsParticles->init();

	explosionParticlesRenderer->init();
	explosionParticlesRenderer->updateBuffer();

	explosionFragmentsParticlesRenderer->init();
	explosionFragmentsParticlesRenderer->updateBuffer();
	//initializeParticlesSystem();
	//loadParticlesSystemRenderer();

	//particlesSystem->init();

	//asteroidsExplosionPattern->init();
	//asteroids	ExplosionFragmentsPattern->init();
}

void AsteroidsManager::initializeParticlesSystem()
{
	explosionParticles = std::make_shared<ParticleSystemV2>();
	explosionParticles->setParticlesCount(100000);
	explosionParticles->setParticlesMaxSpeed(4.0f);
	explosionParticles->setParticlesMaxLifetime(1.7f);
	explosionParticles->setParticlesSize(0.15f);
	explosionParticles->setEvenSpread();
	explosionParticles->setColors(glm::vec3(1.0f, 0.55f, 0.1f), glm::vec3(1.0f, 0.55f, 0.1f));
	explosionParticles->registerCamera(camera);
	explosionParticles->getTransform().setPosition(glm::vec3(0.0f));
	explosionParticles->setBlendingFunctions(GL_ONE, GL_ONE);
	explosionParticles->registerCamera(camera);

	//ParticleSystemFiles data;
	//data.particleTexture = "sprites/asteroid_particle.png";
	//data.vertexShaderFilename = "shaders/particleV2.vert";
	//data.fragmentShaderFilename = "shaders/particle.frag";

	//ModelExternalUniforms uniforms;
	//uniforms.view = camera->getViewPtr();
	//uniforms.projection = projectionPtr;

	//explosionParticles->loadRenderer(data, uniforms);

	explosionFragmentsParticles = std::make_shared<ParticleSystemV2>();
	explosionFragmentsParticles->setParticlesCount(100000);
	explosionFragmentsParticles->setParticlesMaxSpeed(4.5f);
	explosionFragmentsParticles->setParticlesMaxLifetime(2.2f);
	explosionFragmentsParticles->setParticlesSize(0.6f);
	explosionFragmentsParticles->setEvenSpread();
	explosionFragmentsParticles->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	explosionFragmentsParticles->registerCamera(camera);
	explosionFragmentsParticles->getTransform().setPosition(glm::vec3(0.0f));
	explosionFragmentsParticles->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	explosionFragmentsParticles->registerCamera(camera);

	//explosionFragmentsParticles->loadRenderer(data, uniforms);

}

void AsteroidsManager::process()
{
	GameObject::process();

	for (auto asteroid : asteroids)
	{
		asteroid->process();

		if (asteroid->getTransform().getPosition().y < 1.1 * arenaLimits.minY)
			initializeAsteroid(asteroid);

		//if (!asteroid->isActive() && !asteroid->isParticleSystemRunning())
		if(!asteroid->isActive())
		{
			initializeAsteroid(asteroid);
			asteroid->setActive(true);
		}
	}

	//particlesSystem->process();

	//asteroidsExplosionPattern->process();
	//asteroidsExplosionFragmentsPattern->process();
}

void AsteroidsManager::invalidate()
{
	GameObject::invalidate();

	for (auto asteroid : asteroids)
	{
		asteroid->invalidate();
	}

	//particlesSystem->invalidate();

	//asteroidsExplosionPattern->invalidate();
	//asteroidsExplosionFragmentsPattern->invalidate();
}

void AsteroidsManager::registerWorldSpeed(std::shared_ptr<float> speed)
{
	worldSpeed = speed;
}

std::vector<AsteroidPtr>& AsteroidsManager::getAsteroids()
{
	return asteroids;
}

RenderObjectPtr AsteroidsManager::getRenderer()const
{
	return renderer;
}

MultiSourceParticleRendererPtr AsteroidsManager::getExplosionParticlesRenderer()const
{
	return explosionParticlesRenderer;
}

MultiSourceParticleRendererPtr AsteroidsManager::getExplosionFragmentsParticlesRenderer()const
{
	return explosionFragmentsParticlesRenderer;
}

//MultiSourceParticleRendererPtr AsteroidsManager::getParticlesRenderer()const
//{
//	return particlesSystem->getRenderer();
//}

void AsteroidsManager::findLimitPosY()
{
	glm::vec4 pos(0.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 clipPos;
	glm::mat4 VP = (*projectionPtr) * camera->getView();
	clipPos = VP * pos;
	clipPos /= clipPos.w;

	float eps = 0.05f;

	float prevPos = 0.0f;
	float newPos;
	while (std::abs(clipPos.y + 1.0f) > eps) {
		if (clipPos.y < -1.0f)
		{
			newPos = pos.y - (pos.y - prevPos) / 2.0f;
			pos.y = newPos;
		}
		else {
			newPos = pos.y + 2 * (pos.y - prevPos);
			prevPos = pos.y;
			pos.y = newPos;
		}

		clipPos = VP * pos;
		clipPos /= clipPos.w;
	}

	arenaLimits.minY = pos.y;

	pos = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	clipPos = glm::vec4(0.0f);

	prevPos = 0.0f;
	while (std::abs(clipPos.y - 1.0f) > eps)
	{
		if (clipPos.y > 1.0f)
		{
			newPos = pos.y - (pos.y - prevPos) / 2.0f;
			pos.y = newPos;
		}
		else {
			newPos = pos.y + 2 * (pos.y - prevPos);
			prevPos = pos.y;
			pos.y = newPos;
		}

		clipPos = VP * pos;
		clipPos /= clipPos.w;
	}

	arenaLimits.maxY = pos.y;
}

void AsteroidsManager::findLimitPosX()
{
	std::pair<float, float> limits;
	glm::vec4 pos(-1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 clipPos;
	glm::mat4 VP = (*projectionPtr) * camera->getView();
	clipPos = VP * pos;
	clipPos /= clipPos.w;

	float eps = 0.05f;

	float prevPos = 0.0f;
	float newPos;
	while (std::abs(clipPos.x + 1.0f) > eps) {
		if (clipPos.x < -1.0f)
		{
			newPos = pos.x - (pos.x - prevPos) / 2.0f;
			pos.x = newPos;
		}
		else {
			newPos = pos.x + 2 * (pos.x - prevPos);
			prevPos = pos.x;
			pos.x = newPos;
		}

		clipPos = VP * pos;
		clipPos /= clipPos.w;
	}

	arenaLimits.minX = pos.x;

	pos = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	clipPos = glm::vec4(0.0f);

	prevPos = 0.0f;
	while (std::abs(clipPos.x - 1.0f) > eps)
	{
		if (clipPos.x > 1.0f)
		{
			newPos = pos.x - (pos.x - prevPos) / 2.0f;
			pos.x = newPos;
		}
		else {
			newPos = pos.x + 2 * (pos.x - prevPos);
			prevPos = pos.x;
			pos.x = newPos;
		}

		clipPos = VP * pos;
		clipPos /= clipPos.w;
	}

	arenaLimits.maxX = pos.x;
}

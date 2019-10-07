#include "asteroidsmanager.h"

AsteroidsManager::AsteroidsManager()
	: renderer(std::make_shared<MultipleAsteroidsRenderer>())
	, showHitboxFlag(false)
	, timeFromLastAsteroidSpawn(0.0f)
{}

void AsteroidsManager::create(int initialCount, CameraPtr camera, ConstMat4Ptr projectionPtr)
{
	this->camera = camera;
	this->projectionPtr = projectionPtr;
	createPatternAsteroid();
	createPatternAsteroidHitbox();

	initializeParticlesSystem();
	loadParticlesSystemRenderer();
	initializeAsteroidsVector(initialCount);
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

void AsteroidsManager::initializeParticlesSystem()
{
	initializeExplosionParticleSystem();

	initializeExplosionFragmentsParticleSystem();
}

void AsteroidsManager::initializeExplosionParticleSystem()
{
	explosionParticles = std::make_shared<ParticleSystemV2>();
	explosionParticles->setParticlesCount(particlesCountFactor * 100000);
	explosionParticles->setParticlesMaxSpeed(4.0f);
	explosionParticles->setParticlesMaxLifetime(explosionParticlesLifetime * 0.8f);
	explosionParticles->setParticlesSize(0.15f);
	explosionParticles->setEvenSpread();
	explosionParticles->setColors(glm::vec3(1.0f, 0.55f, 0.1f), glm::vec3(1.0f, 0.55f, 0.1f));
	explosionParticles->registerCamera(camera);
	explosionParticles->getTransform().setPosition(glm::vec3(0.0f));
	explosionParticles->setBlendingFunctions(GL_ONE, GL_ONE);
	explosionParticles->registerCamera(camera);
}

void AsteroidsManager::initializeExplosionFragmentsParticleSystem()
{
	explosionFragmentsParticles = std::make_shared<ParticleSystemV2>();
	explosionFragmentsParticles->setParticlesCount(particlesCountFactor * 50000);
	explosionFragmentsParticles->setParticlesMaxSpeed(4.5f);
	explosionFragmentsParticles->setParticlesMaxLifetime(explosionParticlesLifetime);
	explosionFragmentsParticles->setParticlesSize(3.0f);
	explosionFragmentsParticles->setEvenSpread();
	explosionFragmentsParticles->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.5f, 0.5f, 0.5f));
	explosionFragmentsParticles->registerCamera(camera);
	explosionFragmentsParticles->getTransform().setPosition(glm::vec3(0.0f));
	explosionFragmentsParticles->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	explosionFragmentsParticles->registerCamera(camera);
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
	explosionParticlesRenderer->setParticlesCount(explosionParticles->getParticlesCount());

	data.particleTexture = "sprites/smoke.png";
	explosionFragmentsParticlesRenderer = std::make_shared<MultiSourceParticleRenderer>(explosionFragmentsParticles->getParticlesBuffer(),
		explosionFragmentsParticles->getBlendFunctions());
	explosionFragmentsParticlesRenderer->setActive(false);
	explosionFragmentsParticlesRenderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	explosionFragmentsParticlesRenderer->loadTexture(data.particleTexture);
	explosionFragmentsParticlesRenderer->setActive(true);
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
	explosionParticlesRenderer->registerMainUniformData(explosionParticlesUniformData);

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
	explosionFragmentsParticlesRenderer->registerMainUniformData(explosionFragmentsParticlesUniformData);

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

void AsteroidsManager::initializeAsteroidsVector(int initialCount)
{
	findLimitPosY();
	findLimitPosX();

	for (int i = 0; i < initialCount; ++i)
		createAsteroid();

	process();
}

void AsteroidsManager::findLimitPosX()
{
	arenaLimits.minX = findLimitPosXMinVal();
	arenaLimits.maxX = findLimitPosXMaxVal();
}

float AsteroidsManager::findLimitPosXMinVal(float eps)
{
	glm::vec4 pos(-1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 clipPos(0.0f);
	glm::mat4 VP = (*projectionPtr) * camera->getView();
	clipPos = VP * pos;
	clipPos /= clipPos.w;

	glm::vec4 prevPos(0.0f, 0.0f, 0.0f, 1.0f);
	clipPos = VP * prevPos;
	clipPos /= clipPos.w;
	bool initClipPosLowerThanValue = (clipPos.x < -1.0f) ? true : false;

	float signAdjustVar = 1.0f;
	if (initClipPosLowerThanValue) signAdjustVar = -1.0f;

	clipPos = VP * pos;
	clipPos /= clipPos.w;

	float newPos = 0.0f;
	while (std::abs(clipPos.x + 1.0f) > eps) {
		if ((clipPos.x < -1.0f && !initClipPosLowerThanValue) || (clipPos.x > -1.0f && initClipPosLowerThanValue))
		{
			newPos = pos.x + ((std::abs(pos.x - prevPos.x)) / 2.0f) * signAdjustVar;
			pos.x = newPos;
		}
		else {
			newPos = pos.x - 2 * (std::abs(pos.x - prevPos.x)) * signAdjustVar;
			prevPos = pos;
			pos.x = newPos;
		}

		clipPos = VP * pos;
		clipPos /= clipPos.w;
	}

	return pos.x;
}

float AsteroidsManager::findLimitPosXMaxVal(float eps)
{
	glm::vec4 pos = glm::vec4(1.0f, 0.0f, 0.0f, 1.0f);
	glm::vec4 clipPos(0.0f);
	glm::mat4 VP = (*projectionPtr) * camera->getView();
	clipPos = VP * pos;
	clipPos /= clipPos.w;

	glm::vec4 prevPos(0.0f, 0.0f, 0.0f, 1.0f);
	clipPos = VP * prevPos;
	clipPos /= clipPos.w;
	bool initClipPosHigherThanValue = (clipPos.x > 1.0f) ? true : false;
	float signAdjustVar = 1.0f;
	if (initClipPosHigherThanValue) signAdjustVar = -1.0f;

	clipPos = VP * pos;
	clipPos /= clipPos.w;

	float newPos = 0.0f;
	while (std::abs(clipPos.x - 1.0f) > eps)
	{
		if ((clipPos.x > 1.0f && !initClipPosHigherThanValue) || (clipPos.x < 1.0f && initClipPosHigherThanValue))
		{
			newPos = pos.x - ((std::abs(pos.x - prevPos.x)) / 2.0f) * signAdjustVar;
			pos.x = newPos;
		}
		else {
			newPos = pos.x + (2 * (std::abs(pos.x - prevPos.x))) * signAdjustVar;
			prevPos = pos;
			pos.x = newPos;
		}

		clipPos = VP * pos;
		clipPos /= clipPos.w;
	}

	return pos.x;
}

void AsteroidsManager::findLimitPosY()
{
	arenaLimits.minY = findLimitPosYMinVal();
	arenaLimits.maxY = findLimitPosYMaxVal();
}

float AsteroidsManager::findLimitPosYMinVal(float eps)
{
	glm::vec4 pos(0.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 clipPos(0.0f);
	glm::mat4 VP = (*projectionPtr) * camera->getView();

	glm::vec4 prevPos(0.0f, 0.0f, 0.0f, 1.0f);
	clipPos = VP * prevPos;
	clipPos /= clipPos.w;
	bool initClipPosLowerThanValue = ( clipPos.y < -1.0f) ? true : false;
	float signAdjustVar = 1.0f;
	if (initClipPosLowerThanValue) signAdjustVar = -1.0f;

	clipPos = VP * pos;
	clipPos /= clipPos.w;

	float newPos = 0.0f;
	while (std::abs(clipPos.y + 1.0f) > eps) {
		if ((clipPos.y < -1.0f && !initClipPosLowerThanValue) || (clipPos.y > -1.0f && initClipPosLowerThanValue))
		{
			newPos = pos.y + ((std::abs(pos.y - prevPos.y)) / 2.0f) * signAdjustVar;
			pos.y = newPos;
		}
		else {
			newPos = pos.y - (2 * (std::abs(pos.y - prevPos.y))) * signAdjustVar;
			prevPos = pos;
			pos.y = newPos;
		}

		clipPos = VP * pos;
		clipPos /= clipPos.w;
	}

	return pos.y;
}

float AsteroidsManager::findLimitPosYMaxVal(float eps)
{
	glm::vec4 pos = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);
	glm::vec4 clipPos(0.0f);
	glm::mat4 VP = (*projectionPtr) * camera->getView();
	clipPos = VP * pos;
	clipPos /= clipPos.w;

	glm::vec4 prevPos(0.0f, 0.0f, 0.0f, 1.0f);
	clipPos = VP * prevPos;
	clipPos /= clipPos.w;
	bool initClipPosHigherThanValue = (clipPos.y > 1.0f) ? true : false;

	float signAdjustVar = 1.0f;
	if (initClipPosHigherThanValue) signAdjustVar = -1.0f;

	clipPos = VP * pos;
	clipPos /= clipPos.w;

	float newPos = 0.0f;
	while (std::abs(clipPos.y - 1.0f) > eps)
	{
		if ((clipPos.y > 1.0f && !initClipPosHigherThanValue) || (clipPos.y < 1.0f && initClipPosHigherThanValue))
		{
			newPos = pos.y - (((std::abs(pos.y - prevPos.y)) / 2.0f)) * signAdjustVar;
			pos.y = newPos;
		}
		else {
			newPos = pos.y + 2 * (std::abs(pos.y - prevPos.y)) * signAdjustVar;
			prevPos = pos;
			pos.y = newPos;
		}

		clipPos = VP * pos;
		clipPos /= clipPos.w;
	}

	return pos.y;
}

void AsteroidsManager::createAsteroid()
{
	AsteroidPtr asteroid = std::make_shared<Asteroid>();
	asteroid->deepCopy(*patternAsteroid);
	asteroid->setActive(false);

	HitboxObjectPtr hitbox = std::make_shared<HitboxObject>();
	hitbox->deepCopy(*asteroidHitboxPattern);
	hitbox->setName("hitbox_main");

	asteroid->addChild(hitbox);
	asteroid->registerWorldSpeed(worldSpeed);

	asteroid->initParticleSystemsData(explosionParticles->getParticlesMaxLifetime(), explosionFragmentsParticles->getParticlesMaxLifetime());
	explosionParticlesRenderer->registerParticleSystemData(asteroid->getExplosionParticleSystemData());
	explosionFragmentsParticlesRenderer->registerParticleSystemData(asteroid->getExplosionFragmentsParticleSystemData());

	renderer->registerRenderer(asteroid->getRenderer());

	if(showHitboxFlag)
		renderer->registerRenderer(hitbox->getRenderer());

	collisionsManager->registerCollisionObject(asteroid);

	asteroids.push_back(asteroid);
}

void AsteroidsManager::initializeAsteroid(AsteroidPtr asteroid)
{
	std::uniform_real_distribution<> linearSpeedRandX(0.0f, 3.0f);
	std::uniform_real_distribution<> linearSpeedRandY(-15.0f, -5.0f);
	std::uniform_real_distribution<> rotSpeedRand(glm::degrees(asteroidMinAngularRot), glm::degrees(asteroidMaxAngularRot));
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
	asteroid->setHealth(asteroidStandardHealth * spawnSize);
}

void AsteroidsManager::init()
{
	GameObject::init();

	for (auto asteroid : asteroids)
	{
		asteroid->init();
	}
	explosionParticles->init();
	explosionFragmentsParticles->init();

	explosionParticlesRenderer->init();
	explosionParticlesRenderer->updateBuffer();

	explosionFragmentsParticlesRenderer->init();
	explosionFragmentsParticlesRenderer->updateBuffer();
}

void AsteroidsManager::process()
{
	GameObject::process();

	for (auto asteroid : asteroids)
	{
		asteroid->process();

		if (asteroid->getTransform().getPosition().y < 1.7f * arenaLimits.minY)
		{
			asteroid->setActive(false);
		}

		if (asteroid->getTransform().getPosition().x < 1.1f * arenaLimits.minX ||
			asteroid->getTransform().getPosition().x > 1.1f * arenaLimits.maxX)
		{
			initializeAsteroid(asteroid);
			asteroid->setActive(false);
		}
	}

	spawnAsteroids();
}

void AsteroidsManager::spawnAsteroids()
{
	asteroidFrequencySpawn += asteroidSpawnRateAcceleration * Time::deltaTime;
	timeFromLastAsteroidSpawn += Time::deltaTime;

	if (timeFromLastAsteroidSpawn >= 1.0f / asteroidFrequencySpawn)
	{
		bool asteroidActivated = activateAsteroid();
		if (!asteroidActivated)
		{
			createAsteroid();
			activateAsteroid();
		}

		timeFromLastAsteroidSpawn = 0.0f;
	}
}

bool AsteroidsManager::activateAsteroid()
{
	bool activated = false;
	for (auto asteroid : asteroids)
	{
		if (!asteroid->isActive())
		{
			initializeAsteroid(asteroid);
			asteroid->process();
			asteroid->setActive(true);
			activated = true;
			break;
		}
	}

	return activated;
}

void AsteroidsManager::invalidate()
{
	GameObject::invalidate();

	for (auto asteroid : asteroids)
	{
		asteroid->invalidate();
	}

	explosionParticles->invalidate();
	explosionParticlesRenderer->invalidate();

	explosionFragmentsParticles->invalidate();
	explosionFragmentsParticlesRenderer->invalidate();

	patternAsteroid->invalidate();
	asteroidHitboxPattern->invalidate();

	renderer->invalidate();
}

void AsteroidsManager::registerWorldSpeed(std::shared_ptr<float> speed)
{
	worldSpeed = speed;
}

void AsteroidsManager::registerCollisionsManager(CollisionsManagerPtr collisionsManager)
{
	this->collisionsManager = collisionsManager;
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

void AsteroidsManager::restart()
{
	for (auto asteroid : asteroids)
	{
		asteroid->restart();

		std::shared_ptr<Property<GameObject> > prop = asteroid->getProperty("collision");
		if (prop->isUsable())
		{
			ObjectCollision& colObj = static_cast<ObjectCollision&>(*prop);
			colObj.initCollisionDataCleanBuffer();
		}
	}

 	asteroidFrequencySpawn = asteroidDefualtFrequencySpawn;
	timeFromLastAsteroidSpawn = 1.0f / asteroidFrequencySpawn;
}

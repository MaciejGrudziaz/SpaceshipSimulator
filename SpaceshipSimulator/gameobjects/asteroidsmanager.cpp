#include "asteroidsmanager.h"

AsteroidsManager::AsteroidsManager()
	: renderer(std::make_shared<MultipleAsteroidsRenderer>())
{}

void AsteroidsManager::create(int initialCount, ConstMat4Ptr viewPtr, ConstMat4Ptr projectionPtr)
{
	this->viewPtr = viewPtr;
	this->projectionPtr = projectionPtr;
	createPatternAsteroid(viewPtr, projectionPtr);
	createPatternAsteroidHitbox(viewPtr, projectionPtr);
	initializeAsteroidsVector(initialCount);
}

void AsteroidsManager::createPatternAsteroid(ConstMat4Ptr viewPtr, ConstMat4Ptr projectionPtr)
{
	ModelData filesData;
	filesData.modelFilename = "models/asteroid_1.mgr";
	filesData.vertexShaderFilename = "shaders/spaceshipShader.vert";
	filesData.fragmentShaderFilename = "shaders/spaceshipShader.frag";
	filesData.textureFilename = "models/textures/asteroid_1_texture.png";

	ModelExternalUniforms uniforms;
	uniforms.view = viewPtr;
	uniforms.projection = projectionPtr;

	AsteroidPtr asteroid = std::make_shared<Asteroid>();
	asteroid->load(filesData, uniforms);
	asteroid->setName("asteroid");
	asteroid->getTransform().setScale(glm::vec3(0.75f, 0.75f, 0.75f));

	patternAsteroid = asteroid;
}

void AsteroidsManager::createPatternAsteroidHitbox(ConstMat4Ptr viewPtr, ConstMat4Ptr projectionPtr)
{
	HitboxObjectPtr asteroidHitbox = std::make_shared<HitboxObject>();
	asteroidHitbox->setName("hitbox_main");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/asteroidHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = projectionPtr;
	uniforms.view = viewPtr;

	asteroidHitbox->load(data, uniforms);

	asteroidHitboxPattern = asteroidHitbox;
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

		renderer->registerRenderer(asteroid->getRenderer());
		renderer->registerRenderer(hitbox->getRenderer());

		asteroids.push_back(asteroid);
	}
}

void AsteroidsManager::initializeAsteroid(AsteroidPtr asteroid)
{
	std::uniform_real_distribution<> linearSpeedRandX(0.0f, 5.0f);
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
}

void AsteroidsManager::init()
{
	GameObject::init();

	for (auto asteroid : asteroids)
	{
		asteroid->init();
	}
}

void AsteroidsManager::process()
{
	GameObject::process();

	for (auto asteroid : asteroids)
	{
		asteroid->process();

		if (asteroid->getTransform().getPosition().y < 1.1 * arenaLimits.minY)
			initializeAsteroid(asteroid);
	}
}

void AsteroidsManager::invalidate()
{
	GameObject::invalidate();

	for (auto asteroid : asteroids)
	{
		asteroid->invalidate();
	}
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

void AsteroidsManager::findLimitPosY()
{
	glm::vec4 pos(0.0f, -1.0f, 0.0f, 1.0f);
	glm::vec4 clipPos;
	glm::mat4 VP = (*projectionPtr) * (*viewPtr);
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
	glm::mat4 VP = (*projectionPtr) * (*viewPtr);
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

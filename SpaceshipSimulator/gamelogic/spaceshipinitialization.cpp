#include "spaceshipinitialization.h"

void loadSpaceship(GameEngine& engine)
{
	ModelData filesData;
	filesData.modelFilename = "models/spaceship_tex.mgr";
	filesData.vertexShaderFilename = "shaders/spaceshipShader.vert";
	filesData.fragmentShaderFilename = "shaders/spaceshipShader.frag";
	filesData.textureFilename = "models/textures/texture.png";

	ModelExternalUniforms uniforms;
	uniforms.view = engine.getResources()->camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	SpaceshipPtr spaceship = std::make_shared<Spaceship>();
	spaceship->load(filesData, uniforms);
	spaceship->registerCamera(engine.getResources()->camera);
	spaceship->registerProjectionMat(engine.getProjectionMatPtr());
	spaceship->registerEndGameFlag(engine.getEndGameFlag());

	spaceship->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));
	spaceship->getTransform().setDefaultOrientation(glm::vec3(0.0f, 1.0f, 0.0f));

	engine.addRenderer(spaceship->getRenderer());
	engine.addRenderer(spaceship->getBeamsRenderer());
	engine.getResources()->spaceship = spaceship;

	addBodyHitboxToSpaceship(engine);
	addWingHitboxToSpaceship(engine);
	addCocpitHitboxToSpaceship(engine);
	addMainEngineHitboxToSpaceship(engine);

	loadParticlesSytem(engine);
	loadGuns(engine);
	linkSpaceshipDataToGui(engine);
	loadSapceshipDataFromConfigFile(engine);
}

void writeToStdOutBonesNames(SpaceshipPtr spaceship)
{
	std::for_each(spaceship->getModel("spaceship")->skeleton->joints.begin(), spaceship->getModel("spaceship")->skeleton->joints.end(), [](auto joint) {
		std::cout << joint->name << std::endl;
	});
}

void addBodyHitboxToSpaceship(GameEngine& engine)
{
	HitboxObjectPtr bodyHitbox = std::make_shared<HitboxObject>();
	bodyHitbox->setName("hitbox_body_spaceship");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/bodyHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	bodyHitbox->load(data, uniforms);

	if(engine.getShowHitboxesFlag())
		engine.addRenderer(bodyHitbox->getRenderer());

	engine.getResources()->spaceship->addChild(bodyHitbox);
}

void addWingHitboxToSpaceship(GameEngine& engine)
{
	glm::vec3 wingPos(1.95f, -0.15f, 0.0f);

	HitboxObjectPtr rightWing = std::make_shared<HitboxObject>();
	rightWing->setName("hitbox_wing_r_spaceship");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/wingHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	rightWing->load(data, uniforms);
	rightWing->getTransform().setPosition(wingPos);

	if (engine.getShowHitboxesFlag())
		engine.addRenderer(rightWing->getRenderer());

	engine.getResources()->spaceship->addChild(rightWing);

	HitboxObjectPtr leftWing = std::make_shared<HitboxObject>();
	leftWing->setName("hitbox_wing_l_spaceship");
	leftWing->deepCopy(*rightWing);

	wingPos.x = -wingPos.x;
	leftWing->getTransform().setPosition(wingPos);

	if(engine.getShowHitboxesFlag())
		engine.addRenderer(leftWing->getRenderer());

	engine.getResources()->spaceship->addChild(leftWing);
}

void addCocpitHitboxToSpaceship(GameEngine& engine)
{
	glm::vec3 cocpitPos(0.0f, 0.0f, 0.7f);

	HitboxObjectPtr cocpit = std::make_shared<HitboxObject>();
	cocpit->setName("hitbox_cocpit_spaceship");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/cocpitHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	cocpit->load(data, uniforms);
	cocpit->getTransform().setPosition(cocpitPos);

	if (engine.getShowHitboxesFlag())
		engine.addRenderer(cocpit->getRenderer());

	engine.getResources()->spaceship->addChild(cocpit);
}

void addMainEngineHitboxToSpaceship(GameEngine& engine)
{
	glm::vec3 mainEnginePos(0.0f, -2.2f, 0.0f);

	HitboxObjectPtr mainEngine = std::make_shared<HitboxObject>();
	mainEngine->setName("hitbox_mainEngine_spaceship");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/mainEngineHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	mainEngine->load(data, uniforms);
	mainEngine->getTransform().setPosition(mainEnginePos);

	if (engine.getShowHitboxesFlag())
		engine.addRenderer(mainEngine->getRenderer());

	engine.getResources()->spaceship->addChild(mainEngine);
}

void loadGuns(GameEngine& engine)
{
	SpaceshipPtr spaceship = engine.getResources()->spaceship;

	spaceship->loadGuns();
	GameObjectPtr obj = spaceship->getChild("gun_left");
	if (obj->isUsable())
	{
		engine.addRenderer(static_cast<StandardGameObject&>(*obj).getRenderer());
	}
	obj = spaceship->getChild("gun_right");
	if (obj->isUsable())
	{
		engine.addRenderer(static_cast<StandardGameObject&>(*obj).getRenderer());
	}
}

void loadParticlesSytem(GameEngine& engine)
{
	SpaceshipPtr spaceship = engine.getResources()->spaceship;

	ParticleSystemV2Ptr mainEngine = std::make_shared<ParticleSystemV2>();
	mainEngine->setName("particles_mainEngine");
	mainEngine->setParticlesCount(10000);
	mainEngine->setParticlesMaxSpeed(10.0f);
	mainEngine->setParticlesMaxLifetime(0.5f);
	mainEngine->setParticlesSize(1.0f);
	mainEngine->setParticlesSpreadCone(-30.0f, 30.0f);
	mainEngine->setColors(glm::vec3(1.0f, 0.92f, 0.01f), glm::vec3(0.2f, 0.2f, 0.2f));
	mainEngine->setBlendingFunctions(GL_ONE, GL_ONE);
	mainEngine->getTransform().setRotation(glm::vec3(-45.0f, 0.0f, 0.0f));
	mainEngine->getTransform().setPosition(glm::vec3(0.0f, -2.7f, 0.0f));

	mainEngine->registerCamera(engine.getResources()->camera);
	
	ParticleSystemFiles data;
	data.particleTexture = "sprites/smoke.png";
	data.vertexShaderFilename = "shaders/particleV2.vert";
	data.fragmentShaderFilename = "shaders/particle.frag";

	ModelExternalUniforms uniforms;
	uniforms.view = engine.getResources()->camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	mainEngine->loadRenderer(data, uniforms);

	spaceship->addChild(mainEngine);
	engine.addRenderer(mainEngine->getRenderer());

	//data.vertexShaderFilename = "shaders/particle.vert";
	ParticleSystemV2Ptr mainEngineSmoke = std::make_shared<ParticleSystemV2>();
	mainEngineSmoke->setName("particles_mainEngineSmoke");
	mainEngineSmoke->setParticlesCount(10000);
	mainEngineSmoke->setParticlesMaxSpeed(3.0f);
	mainEngineSmoke->setParticlesMaxLifetime(1.0f);
	mainEngineSmoke->setParticlesSize(1.2f);
	mainEngineSmoke->setParticlesSpreadCone(-110.0f, 110.0f);
	mainEngineSmoke->setColors(glm::vec3(1.0f, 0.92f, 0.01f), glm::vec3(1.0f, 1.0f, 1.0f));
	mainEngineSmoke->getTransform().setRotation(glm::vec3(-45.0f, 0.0f, 0.0f));
	mainEngineSmoke->getTransform().setPosition(glm::vec3(0.0f, -2.7f, 0.0f));

	mainEngineSmoke->registerCamera(engine.getResources()->camera);
	mainEngineSmoke->loadRenderer(data, uniforms);

	spaceship->addChild(mainEngineSmoke);
	engine.addRenderer(mainEngineSmoke->getRenderer());

	ParticleSystemV2Ptr lowerRightEngine = std::make_shared<ParticleSystemV2>();
	lowerRightEngine->setName("particles_lowerRightEngine");
	lowerRightEngine->setParticlesCount(5000);
	lowerRightEngine->setParticlesMaxSpeed(5.0f);
	lowerRightEngine->setParticlesMaxLifetime(0.4f);
	lowerRightEngine->setParticlesSize(0.6f);
	lowerRightEngine->setParticlesSpreadCone(-20.0f, 20.0f);
	lowerRightEngine->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	lowerRightEngine->getTransform().setRotation(glm::vec3(-45.0f, 0.0f, 30.0f));
	lowerRightEngine->getTransform().setPosition(glm::vec3(1.6f, -1.78f, 0.0f));

	lowerRightEngine->registerCamera(engine.getResources()->camera);
	lowerRightEngine->loadRenderer(data, uniforms);

	spaceship->addChild(lowerRightEngine);
	engine.addRenderer(lowerRightEngine->getRenderer());

	ParticleSystemV2Ptr lowerLeftEngine = std::make_shared<ParticleSystemV2>();
	lowerLeftEngine->setName("particles_lowerLeftEngine");
	lowerLeftEngine->setParticlesCount(5000);
	lowerLeftEngine->setParticlesMaxSpeed(5.0f);
	lowerLeftEngine->setParticlesMaxLifetime(0.4f);
	lowerLeftEngine->setParticlesSize(0.6f);
	lowerLeftEngine->setParticlesSpreadCone(-20.0f, 20.0f);
	lowerLeftEngine->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	lowerLeftEngine->getTransform().setRotation(glm::vec3(-45.0f, 0.0f, -30.0f));
	lowerLeftEngine->getTransform().setPosition(glm::vec3(-1.6f, -1.78f, 0.0f));

	lowerLeftEngine->registerCamera(engine.getResources()->camera);
	lowerLeftEngine->loadRenderer(data, uniforms);

	spaceship->addChild(lowerLeftEngine);
	engine.addRenderer(lowerLeftEngine->getRenderer());

	ParticleSystemV2Ptr upperLeftEngine = std::make_shared<ParticleSystemV2>();
	upperLeftEngine->setName("particles_upperLeftEngine");
	upperLeftEngine->setParticlesCount(5000);
	upperLeftEngine->setParticlesMaxSpeed(5.0f);
	upperLeftEngine->setParticlesMaxLifetime(0.4f);
	upperLeftEngine->setParticlesSize(0.6f);
	upperLeftEngine->setParticlesSpreadCone(-20.0f, 20.0f);
	upperLeftEngine->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	upperLeftEngine->getTransform().setRotation(glm::vec3(-45.0f, 0.0f, -82.5f));
	upperLeftEngine->getTransform().setPosition(glm::vec3(-1.15f, 2.9f, 0.0f));

	upperLeftEngine->registerCamera(engine.getResources()->camera);
	upperLeftEngine->loadRenderer(data, uniforms);

	spaceship->addChild(upperLeftEngine);
	engine.addRenderer(upperLeftEngine->getRenderer());

	ParticleSystemV2Ptr upperRightEngine = std::make_shared<ParticleSystemV2>();
	upperRightEngine->setName("particles_upperRightEngine");
	upperRightEngine->setParticlesCount(5000);
	upperRightEngine->setParticlesMaxSpeed(5.0f);
	upperRightEngine->setParticlesMaxLifetime(0.3f);
	upperRightEngine->setParticlesSize(0.4f);
	upperRightEngine->setParticlesSpreadCone(-15.0f, 15.0f);
	upperRightEngine->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(1.0f, 1.0f, 1.0f));
	upperRightEngine->getTransform().setRotation(glm::vec3(-45.0f, 0.0f, 82.5f));
	upperRightEngine->getTransform().setPosition(glm::vec3(1.15f, 2.9f, 0.0f));

	upperRightEngine->registerCamera(engine.getResources()->camera);
	upperRightEngine->loadRenderer(data, uniforms);

	spaceship->addChild(upperRightEngine);
	engine.addRenderer(upperRightEngine->getRenderer());
}

void registerSpaceshipCollisionSystem(GameEngine& engine)
{
	engine.getResources()->collisionsManager->registerCollisionObject(engine.getResources()->spaceship);
	engine.getResources()->collisionsManager->registerHitDetectionObject(engine.getResources()->spaceship);
}

void linkSpaceshipDataToGui(GameEngine& engine)
{
	engine.getGui()->registerSpaceship(engine.getResources()->spaceship);
	engine.getGui()->registerLifeFracValue(engine.getResources()->spaceship->getLifeFracValuePtr());
	engine.getGui()->registerShieldFracValue(engine.getResources()->spaceship->getShieldFracValuePtr());
	engine.getGui()->registerPointsCounter(engine.getResources()->spaceship->getPointsScorePtr());
}

void loadSapceshipDataFromConfigFile(GameEngine& engine)
{
	GameConfigData config = engine.getConfigData();
	SpaceshipPtr spaceship = engine.getResources()->spaceship;

	spaceship->setMainEngineThrust(config.mainEngineThrust);
	spaceship->setManeuverEngineThrust(config.maneuverEngineThrust);
	spaceship->setMass(config.spaceshipMass);
	spaceship->setManeuverEngineForceVecLength(config.maneuverEngineForceArmLength);
	spaceship->setAdvancedFlightMode(config.advancedFlightMode);
	spaceship->setMaxLifeValue(config.spaceshipHealth);
	spaceship->setMaxShieldValue(config.spaceshipShieldsHealth);
	spaceship->setShotDamage(config.spaceshipShotDamage);
	spaceship->setFireRate(config.spaceshipFireRate);
	spaceship->setLaserBeamSpeed(config.spaceshipLaserShotSpeed);
	spaceship->setMouseInput(config.mouseInput);
}

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
}

void writeToStdOutBonesNames(SpaceshipPtr spaceship)
{
	std::for_each(spaceship->getModel("spaceship")->skeleton->joints.begin(), spaceship->getModel("spaceship")->skeleton->joints.end(), [](auto joint) {
		std::cout << joint->name << std::endl;
	});
}

void addBodyHitboxToSpaceship(GameEngine& engine)
{
	HitboxObjectPtr testHitbox = std::make_shared<HitboxObject>();
	testHitbox->setName("hitbox_body");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/bodyHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	testHitbox->load(data, uniforms);

	engine.addRenderer(testHitbox->getRenderer());

	engine.getResources()->spaceship->addChild(testHitbox);
}

void addWingHitboxToSpaceship(GameEngine& engine)
{
	glm::vec3 wingPos(1.95f, -0.15f, 0.0f);

	HitboxObjectPtr rightWing = std::make_shared<HitboxObject>();
	rightWing->setName("hitbox_wing_r");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/wingHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	rightWing->load(data, uniforms);
	rightWing->getTransform().setPosition(wingPos);

	engine.addRenderer(rightWing->getRenderer());
	engine.getResources()->spaceship->addChild(rightWing);

	HitboxObjectPtr leftWing = std::make_shared<HitboxObject>();
	leftWing->setName("hitbox_wing_l");
	leftWing->deepCopy(*rightWing);

	//leftWing->load(rightWing->getHitbox(), data, uniforms);

	wingPos.x = -wingPos.x;
	leftWing->getTransform().setPosition(wingPos);

	engine.addRenderer(leftWing->getRenderer());
	engine.getResources()->spaceship->addChild(leftWing);
}

void addCocpitHitboxToSpaceship(GameEngine& engine)
{
	glm::vec3 cocpitPos(0.0f, 0.0f, 0.7f);

	HitboxObjectPtr cocpit = std::make_shared<HitboxObject>();
	cocpit->setName("hitbox_cocpit");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/cocpitHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	cocpit->load(data, uniforms);
	cocpit->getTransform().setPosition(cocpitPos);

	engine.addRenderer(cocpit->getRenderer());
	engine.getResources()->spaceship->addChild(cocpit);
}

void addMainEngineHitboxToSpaceship(GameEngine& engine)
{
	glm::vec3 mainEnginePos(0.0f, -2.2f, 0.0f);

	HitboxObjectPtr mainEngine = std::make_shared<HitboxObject>();
	mainEngine->setName("hitbox_mainEngine");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/mainEngineHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	mainEngine->load(data, uniforms);
	mainEngine->getTransform().setPosition(mainEnginePos);

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

	ParticleSystemPtr mainEngine = std::make_shared<ParticleSystem>();
	mainEngine->setName("particles_mainEngine");
	mainEngine->setParticlesCount(2000);
	mainEngine->setParticlesMaxSpeed(10.0f);
	mainEngine->setParticlesMaxLifetime(0.3f);
	mainEngine->setParticlesSize(0.5f);
	mainEngine->setParticlesSpreadCone(-30.0f, 30.0f);
	mainEngine->setColors(glm::vec3(1.0f, 0.92f, 0.01f), glm::vec3(0.2f, 0.2f, 0.2f));
	mainEngine->setBlendingFunctions(GL_ONE, GL_ONE);
	mainEngine->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	mainEngine->getTransform().setPosition(glm::vec3(0.0f, -2.7f, 0.0f));

	mainEngine->registerCamera(engine.getResources()->camera);

	ParticleSystemData data;
	data.particleTexture = "sprites/smoke.png";
	data.vertexShaderFilename = "shaders/particle.vert";
	data.fragmentShaderFilename = "shaders/particle.frag";

	ModelExternalUniforms uniforms;
	uniforms.view = engine.getResources()->camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	mainEngine->load(data, uniforms);
	//mainEngine->launch();

	spaceship->addChild(mainEngine);
	engine.addRenderer(mainEngine->getRenderer());

	ParticleSystemPtr mainEngineSmoke = std::make_shared<ParticleSystem>();
	mainEngineSmoke->setName("particles_mainEngineSmoke");
	mainEngineSmoke->setParticlesCount(2000);
	mainEngineSmoke->setParticlesMaxSpeed(3.0f);
	mainEngineSmoke->setParticlesMaxLifetime(0.6f);
	mainEngineSmoke->setParticlesSize(0.8f);
	mainEngineSmoke->setParticlesSpreadCone(-110.0f, 110.0f);
	mainEngineSmoke->setColors(glm::vec3(1.0f, 0.92f, 0.01f), glm::vec3(1.0f, 1.0f, 1.0f));
	mainEngineSmoke->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	mainEngineSmoke->getTransform().setPosition(glm::vec3(0.0f, -2.7f, 0.0f));

	mainEngineSmoke->registerCamera(engine.getResources()->camera);
	mainEngineSmoke->load(data, uniforms);
	//mainEngineSmoke->launch();

	spaceship->addChild(mainEngineSmoke);
	engine.addRenderer(mainEngineSmoke->getRenderer());

	ParticleSystemPtr lowerRightEngine = std::make_shared<ParticleSystem>();
	lowerRightEngine->setName("particles_lowerRightEngine");
	lowerRightEngine->setParticlesCount(2000);
	lowerRightEngine->setParticlesMaxSpeed(5.0f);
	lowerRightEngine->setParticlesMaxLifetime(0.3f);
	lowerRightEngine->setParticlesSize(0.5f);
	lowerRightEngine->setParticlesSpreadCone(-45.0f, 45.0f);
	lowerRightEngine->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	lowerRightEngine->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, 45.0f));
	lowerRightEngine->getTransform().setPosition(glm::vec3(1.6f, -1.78f, 0.0f));

	lowerRightEngine->registerCamera(engine.getResources()->camera);
	lowerRightEngine->load(data, uniforms);
	//lowerRightEngine->launch();

	spaceship->addChild(lowerRightEngine);
	engine.addRenderer(lowerRightEngine->getRenderer());

	ParticleSystemPtr lowerLeftEngine = std::make_shared<ParticleSystem>();
	lowerLeftEngine->setName("particles_lowerLeftEngine");
	lowerLeftEngine->setParticlesCount(2000);
	lowerLeftEngine->setParticlesMaxSpeed(5.0f);
	lowerLeftEngine->setParticlesMaxLifetime(0.3f);
	lowerLeftEngine->setParticlesSize(0.5f);
	lowerLeftEngine->setParticlesSpreadCone(-45.0f, 45.0f);
	lowerLeftEngine->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	lowerLeftEngine->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, -45.0f));
	lowerLeftEngine->getTransform().setPosition(glm::vec3(-1.6f, -1.78f, 0.0f));

	lowerLeftEngine->registerCamera(engine.getResources()->camera);
	lowerLeftEngine->load(data, uniforms);
	//lowerLeftEngine->launch();

	spaceship->addChild(lowerLeftEngine);
	engine.addRenderer(lowerLeftEngine->getRenderer());

	ParticleSystemPtr upperLeftEngine = std::make_shared<ParticleSystem>();
	upperLeftEngine->setName("particles_upperLeftEngine");
	upperLeftEngine->setParticlesCount(2000);
	upperLeftEngine->setParticlesMaxSpeed(5.0f);
	upperLeftEngine->setParticlesMaxLifetime(0.3f);
	upperLeftEngine->setParticlesSize(0.5f);
	upperLeftEngine->setParticlesSpreadCone(-45.0f, 45.0f);
	upperLeftEngine->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	upperLeftEngine->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, -135.0f));
	upperLeftEngine->getTransform().setPosition(glm::vec3(-1.15f, 2.9f, 0.0f));

	upperLeftEngine->registerCamera(engine.getResources()->camera);
	upperLeftEngine->load(data, uniforms);
	//upperLeftEngine->launch();

	spaceship->addChild(upperLeftEngine);
	engine.addRenderer(upperLeftEngine->getRenderer());

	ParticleSystemPtr upperRightEngine = std::make_shared<ParticleSystem>();
	upperRightEngine->setName("particles_upperRightEngine");
	upperRightEngine->setParticlesCount(2000);
	upperRightEngine->setParticlesMaxSpeed(5.0f);
	upperRightEngine->setParticlesMaxLifetime(0.3f);
	upperRightEngine->setParticlesSize(0.5f);
	upperRightEngine->setParticlesSpreadCone(-45.0f, 45.0f);
	upperRightEngine->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.2f, 0.2f, 0.2f));
	upperRightEngine->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, 135.0f));
	upperRightEngine->getTransform().setPosition(glm::vec3(1.15f, 2.9f, 0.0f));

	upperRightEngine->registerCamera(engine.getResources()->camera);
	upperRightEngine->load(data, uniforms);
	//upperRightEngine->launch();

	spaceship->addChild(upperRightEngine);
	engine.addRenderer(upperRightEngine->getRenderer());
}

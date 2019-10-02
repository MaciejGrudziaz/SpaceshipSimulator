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

	spaceship->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

	engine.addRenderer(spaceship->getRenderer());
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

	ParticleSystemPtr particles = std::make_shared<ParticleSystem>();
	particles->setName("particles");
	particles->setParticlesCount(2000);
	particles->setParticlesMaxSpeed(10.0f);
	particles->setParticlesMaxLifetime(0.3f);
	particles->setParticlesSize(0.8f);
	particles->setParticlesSpreadCone(-30.0f, 30.0f);
	particles->setColors(glm::vec3(1.0f, 0.92f, 0.01f), glm::vec3(0.2f, 0.2f, 0.2f));
	particles->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE);
	particles->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	particles->getTransform().setPosition(glm::vec3(0.0f, -2.7f, 0.0f));

	particles->registerCamera(engine.getResources()->camera);

	ParticleSystemData data;
	data.particleTexture = "sprites/smoke.png";
	data.vertexShaderFilename = "shaders/particle.vert";
	data.fragmentShaderFilename = "shaders/particle.frag";

	ModelExternalUniforms uniforms;
	uniforms.view = engine.getResources()->camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	particles->load(data, uniforms);
	particles->launch();

	spaceship->addChild(particles);
	engine.addRenderer(particles->getRenderer());

	ParticleSystemPtr particles2 = std::make_shared<ParticleSystem>();
	particles2->setName("particles2");
	particles2->setParticlesCount(2000);
	particles2->setParticlesMaxSpeed(5.0f);
	particles2->setParticlesMaxLifetime(0.3f);
	particles2->setParticlesSize(0.8f);
	particles2->setParticlesSpreadCone(-110.0f, 110.0f);
	particles2->setColors(glm::vec3(1.0f, 0.92f, 0.01f), glm::vec3(1.0f, 1.0f, 1.0f));
	particles2->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	particles2->getTransform().setPosition(glm::vec3(0.0f, -2.7f, 0.0f));

	particles2->registerCamera(engine.getResources()->camera);
	particles2->load(data, uniforms);
	particles2->launch();

	spaceship->addChild(particles2);
	engine.addRenderer(particles2->getRenderer());
}

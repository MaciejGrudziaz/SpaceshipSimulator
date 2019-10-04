#include "asteroidinitialization.h"

void loadPatternAsteroid(GameEngine& engine)
{
	ModelData filesData;
	filesData.modelFilename = "models/asteroid_1.mgr";
	filesData.vertexShaderFilename = "shaders/spaceshipShader.vert";
	filesData.fragmentShaderFilename = "shaders/spaceshipShader.frag";
	filesData.textureFilename = "models/textures/asteroid_1_texture.png";

	ModelExternalUniforms uniforms;
	uniforms.view = engine.getResources()->camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	StandardGameObjectPtr asteroid = std::make_shared<StandardGameObject>();
	asteroid->load(filesData, uniforms);
	asteroid->setName("asteroid");
	asteroid->getTransform().setScale(glm::vec3(0.75f, 0.75f, 0.75f));

	engine.getResources()->asteroidPattern = asteroid;

	addHitboxToAsteroid(engine);
}

void addHitboxToAsteroid(GameEngine& engine)
{
	HitboxObjectPtr asteroidHitbox = std::make_shared<HitboxObject>();
	asteroidHitbox->setName("hitbox_main");

	HitboxObjectData data;
	data.vertexShaderFilename = "shaders/standardHitbox.vert";
	data.fragmentShaderFilename = "shaders/standardHitbox.frag";
	data.hitboxFilename = "hitboxes/asteroidHitbox.hbx";

	ModelExternalUniforms uniforms;
	uniforms.projection = engine.getProjectionMatPtr();
	uniforms.view = engine.getCamera()->getViewPtr();

	asteroidHitbox->load(data, uniforms);

	engine.getResources()->asteroidHitboxPattern = asteroidHitbox;
}

void loadAsteroids(GameEngine& engine)
{
	AsteroidsManagerPtr asteroids = std::make_shared<AsteroidsManager>();
	asteroids->registerWorldSpeed(engine.getResources()->worldSpeed);
	asteroids->create(1, engine.getResources()->camera, engine.getProjectionMatPtr());

	engine.addRenderer(asteroids->getRenderer());
	engine.getResources()->asteroids = asteroids;
}

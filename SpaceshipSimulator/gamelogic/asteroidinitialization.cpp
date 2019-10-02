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
	//engine.addRenderer(asteroid->getRenderer());

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

	//engine.addRenderer(asteroidHitbox->getRenderer());
	//engine.getResources()->asteroidPattern->addChild(asteroidHitbox);
}

void loadAsteroids(GameEngine& engine)
{
	for (int i = 0; i < 10; ++i)
	{
		glm::vec3 randPos;
		randPos.x = (rand() % 40) - 20;
		randPos.y = (rand() % 10);

		StandardGameObjectPtr asteroid = std::make_shared<StandardGameObject>();
		asteroid->deepCopy(*(engine.getResources()->asteroidPattern));

		HitboxObjectPtr hitbox = std::make_shared<HitboxObject>();
		hitbox->deepCopy(*(engine.getResources()->asteroidHitboxPattern));
		hitbox->setName("hitbox_main");

		asteroid->addChild(hitbox);
		asteroid->getTransform().setPosition(randPos);

		engine.getResources()->asteroids.push_back(asteroid);
		engine.addRenderer(asteroid->getRenderer());
		engine.addRenderer(hitbox->getRenderer());
	}
}

#include "initialization.h"

void setCamera(GameEngine& engine)
{
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->getTransform().setPosition(glm::vec3(0.0f, 10.0f, 30.0f));

	engine.setCamera(camera);
	engine.getResources()->camera = camera;
}

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

	StandardGameObjectPtr spaceship = std::make_shared<StandardGameObject>();
	spaceship->load(filesData, uniforms);

	//std::for_each(spaceship->getModel("spaceship")->skeleton->joints.begin(), spaceship->getModel("spaceship")->skeleton->joints.end(), [](auto joint) {
	//	std::cout << joint->name << std::endl;
	//});

	engine.addRenderer(spaceship->getRenderer());
	engine.getResources()->spaceship = spaceship;
}

void loadAsteroids(GameEngine& engine)
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

	for (int i = 0; i < 3; ++i)
	{
		glm::vec3 randPos;
		randPos.x = (rand() % 40) - 20;
		randPos.y = (rand() % 10) + 10;

		StandardGameObjectPtr newAsteroid = std::make_shared<StandardGameObject>();
		newAsteroid->deepCopy(*asteroid);
		newAsteroid->getTransform().setPosition(randPos);

		engine.getResources()->asteroids.push_back(newAsteroid);
		engine.addRenderer(newAsteroid->getRenderer());
	}
}

void loadTestAsteroid(GameEngine& engine)
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

	GameObjectPtr parent = std::make_shared<GameObject>();
	parent->addChild(asteroid);

	parent->getTransform().setPosition(glm::vec3(10.0f, 10.0f, 0.0f));

	engine.getResources()->testAsteroid = parent;
	engine.addRenderer(asteroid->getRenderer());
}

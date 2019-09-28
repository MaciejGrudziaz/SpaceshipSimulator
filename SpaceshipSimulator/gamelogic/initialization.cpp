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
	SpaceshipData filesData;
	filesData.modelFilename = "models/spaceship_tex.mgr";
	filesData.vertexShaderFilename = "shaders/spaceshipShader.vert";
	filesData.fragmentShaderFilename = "shaders/spaceshipShader.frag";
	filesData.textureFilename = "models/textures/texture.png";

	SpaceshipUniforms uniforms;
	uniforms.view = engine.getResources()->camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	std::shared_ptr<Spaceship> spaceship = std::make_shared<Spaceship>();
	spaceship->load(filesData, uniforms);

	engine.addRenderer(spaceship->getRenderer());
	engine.getResources()->spaceship = spaceship;
}

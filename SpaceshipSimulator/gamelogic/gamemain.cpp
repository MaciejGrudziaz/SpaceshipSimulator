#include "gamemain.h"

GAME_INIT(engine)
{
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 3.0f));

	engine.registerCamera(camera);
	engine.getResources()->camera = camera;

	SpaceshipData filesData;
	filesData.modelFilename = "models/spaceship_tex.mgr";
	filesData.vertexShaderFilename = "shaders/spaceshipShader.vert";
	filesData.fragmentShaderFilename = "shaders/spaceshipShader.frag";
	filesData.textureFilename = "models/textures/texture.png";

	SpaceshipUniforms uniforms;
	uniforms.view = camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	std::shared_ptr<Spaceship> spaceship = std::make_shared<Spaceship>();
	spaceship->load(filesData, uniforms);

	engine.addRenderer(spaceship->getRenderer());
	engine.getResources()->spaceship = spaceship;
}

GAME_PROCESS(engine)
{
	static GameResourcesPtr resources = engine.getResources();

	glm::vec3 camPos = resources->camera->getTransform().getPosition();
	camPos.z += 0.5f * Time::deltaTime;

	resources->camera->getTransform().setPosition(camPos);

	resources->spaceship->process();
}

GAME_END(engine)
{

}

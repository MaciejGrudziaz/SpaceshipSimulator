#include "initialization.h"

void setCamera(GameEngine& engine)
{
	std::shared_ptr<Camera> camera = std::make_shared<Camera>();
	camera->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 30.0f));

	engine.setCamera(camera);
	engine.getResources()->camera = camera;
}

#include "rotation.h"

void ControlRotation::init()
{
	rotSpeed = 100.0f;
}

void ControlRotation::process()
{
	glm::vec3 rot;
	if (GameEngine::getKey(GLFW_KEY_W) == GLFW_PRESS)
		rot.x -= 1.0f;
	if (GameEngine::getKey(GLFW_KEY_S) == GLFW_PRESS)
		rot.x += 1.0f;
	if (GameEngine::getKey(GLFW_KEY_D) == GLFW_PRESS)
		rot.y += 1.0f;
	if (GameEngine::getKey(GLFW_KEY_A) == GLFW_PRESS)
		rot.y -= 1.0f;

	rot *= rotSpeed * Time::deltaTime;

	glm::vec3 currObjRot = object.getTransform().getRotation();
	rot += currObjRot;
	if (rot.x > 360.0f) rot.x -= 360.0f;
	if (rot.y > 360.0f) rot.y -= 360.0f;

	object.getTransform().setRotation(rot);
}

void ControlRotation::invalidate()
{}


void ControlZoom::init()
{
	zoomSpeed = 20.0f;
}

void ControlZoom::process()
{
	glm::vec3 objPos = object.getTransform().getPosition();

	float zoomVal = 0.0f;

	if (GameEngine::getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
		zoomVal += 1.0f;
	if (GameEngine::getKey(GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS)
		zoomVal -= 1.0f;

	zoomVal *= zoomSpeed * Time::deltaTime;

	objPos.z += zoomVal;

	object.getTransform().setPosition(objPos);
}

void ControlZoom::invalidate()
{}

void loadControlRotationToSpaceship(GameEngine& engine)
{
	engine.getResources()->spaceship->addProperty<ControlRotation>("rotation");
}

void loadControlZoomToSpaceship(GameEngine& engine)
{
	engine.getResources()->spaceship->addProperty<ControlZoom>("zoom");
}

void loadControlRotationToTestAsteroid(GameEngine& engine)
{
	engine.getResources()->asteroidPattern->addProperty<ControlRotation>("rotation");
}

void loadControlZoomToTestAsteroid(GameEngine& engine)
{
	engine.getResources()->asteroidPattern->addProperty<ControlZoom>("zoom");
}

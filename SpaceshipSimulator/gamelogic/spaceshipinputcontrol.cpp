#include "spaceshipinputcontrol.h"

void PlayerInput::init()
{
	prevPos = GameEngine::getCursorPos();
	speed = 10.0f;
	rotSpeed = 2.0f;
}

void PlayerInput::process()
{
	processKeyInput();

	processMouseInput();
}

void PlayerInput::processKeyInput()
{
	glm::vec3 moveVec(0.0f);

	if (GameEngine::getKey(GLFW_KEY_A) == GLFW_PRESS)
		moveVec.x -= 1.0f;
	if (GameEngine::getKey(GLFW_KEY_D) == GLFW_PRESS)
		moveVec.x += 1.0f;


	if (moveVec != glm::vec3(0.0f))
		moveVec = glm::normalize(moveVec);

	Spaceship& spaceship = static_cast<Spaceship&>(object);
	spaceship.setInputMoveVec(moveVec);
}

void PlayerInput::processMouseInput()
{
	GameEngine::CursorPos currPos = GameEngine::getCursorPos();
	GameEngine::CursorPos diff = currPos - prevPos;
	prevPos = currPos;

	float rotVal = -diff.x;

	Spaceship& spaceship = static_cast<Spaceship&>(object);
	spaceship.setInputRotation(rotVal);
}

void PlayerInput::invalidate()
{}

void registerInput(GameEngine& engine)
{
	engine.getResources()->spaceship->addProperty<PlayerInput>("input");
}

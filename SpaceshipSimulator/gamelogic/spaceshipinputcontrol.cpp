#include "spaceshipinputcontrol.h"

void PlayerInput::init()
{
	Spaceship& spaceship = static_cast<Spaceship&>(object);
	mouseInput = spaceship.getMouseInput();

	prevPos = GameEngine::getCursorPos();
	speed = 10.0f;
	rotSpeed = 2.0f;
}

void PlayerInput::process()
{
	processKeyInput();

	if(mouseInput)
		processMouseInput();
}

void PlayerInput::processKeyInput()
{
	static Spaceship& spaceship = static_cast<Spaceship&>(object);
	glm::vec3 moveVec(0.0f);

	if (GameEngine::getKey(GLFW_KEY_A) == GLFW_PRESS)
		moveVec.x -= 1.0f;
	if (GameEngine::getKey(GLFW_KEY_D) == GLFW_PRESS)
		moveVec.x += 1.0f;

	if (!mouseInput)
	{
		float rotVal = 0.0f;
		if (GameEngine::getKey(GLFW_KEY_RIGHT) == GLFW_PRESS)
			rotVal = -200.0f;
		if (GameEngine::getKey(GLFW_KEY_LEFT) == GLFW_PRESS)
			rotVal = 200.0f;

		spaceship.setInputRotation(rotVal);
	}


	if (moveVec != glm::vec3(0.0f))
		moveVec = glm::normalize(moveVec);

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

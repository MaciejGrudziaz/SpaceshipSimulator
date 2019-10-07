#pragma once

#include "../gameengine.h"
#include "../gameobjects/gameresources.h"

class PlayerInput : public Property<GameObject>
{
public:
	PlayerInput(const std::string& name, GameObject& object)
		: Property<GameObject>(name, object)
	{}

	void init()override;
	void process()override;
	void invalidate()override;

private:
	float speed;
	float rotSpeed;
	bool mouseInput;

	GameEngine::CursorPos prevPos;

	void processKeyInput();
	void processMouseInput();
};

void registerInput(GameEngine& engine);

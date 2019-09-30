#pragma once
#include "../gameengine.h"
#include "../gameobjects/gameresources.h"

class ControlRotation : public Property<GameObject>
{
public:
	ControlRotation(const std::string& name, GameObject& object)
		: Property<GameObject>(name, object)
	{}

	void init()override;

	void process()override;

	void invalidate()override;

private:
	float rotSpeed;
};

class ControlZoom : public Property<GameObject>
{
public:
	ControlZoom(const std::string& name, GameObject& object)
		: Property<GameObject>(name, object)
	{}

	void init()override;

	void process()override;

	void invalidate()override;

private:
	float zoomSpeed;
};

void loadControlRotationToSpaceship(GameEngine& engine);
void loadControlZoomToSpaceship(GameEngine& engine);
void loadControlRotationToTestAsteroid(GameEngine& engine);
void loadControlZoomToTestAsteroid(GameEngine& engine);

#include "spaceshipenginescontrol.h"

void EnginesControl::init()
{

}
void EnginesControl::process()
{
	if (!initalized)
	{
		prevPos = object.getTransform().getPosition();
		initalized = true;
	}


	glm::vec3 moveDir = object.getTransform().getPosition() - prevPos;

	IdxPair launchEngines(-1, -1);
	if (!areEqual(moveDir, glm::vec3(0.0f)))
	{
		moveDir = glm::normalize(moveDir);

		for (int i = 0; i < dirVectors.size(); ++i)
		{
			glm::vec3 locDirVec = (object.getTransform().getRotationQuat() * dirVectors[i]);
			float angle = glm::degrees(glm::acos(glm::dot(locDirVec, moveDir)));

			if (angle < 22.5f) {
				launchEngines = directionEngines[i];
				break;
			}
		}
	}

	processMainEngine(true);

	processManeuverEngine(launchEngines.first, true);
	processManeuverEngine(launchEngines.second, true);

	for (int i = 2; i < enginesNames.size(); ++i)
	{
		if (i != launchEngines.first && i != launchEngines.second)
		{
			processManeuverEngine(i, false);
		}
	}

	prevPos = object.getTransform().getPosition();
}

void EnginesControl::processMainEngine(bool val)
{
	glm::vec3 forwardDir(0.0f, 1.0f, 0.0f);
	float angle = glm::degrees(glm::acos(glm::dot(forwardDir, object.getTransform().getOrientation())));
	GameObjectPtr obj = object.getChild(enginesNames[0]);
	float limitAngle = 90.0f;

	glm::vec3 moveDir = object.getTransform().getPosition() - prevPos;
	float dotProd;
	if (areEqual(moveDir, glm::vec3(0.0f))) dotProd = 0.0f;
	else dotProd = glm::dot(glm::normalize(moveDir), object.getTransform().getOrientation());

	if (obj->isUsable())
	{
		ParticleSystem& mainEngine = static_cast<ParticleSystem&>(*obj);
		if (angle < limitAngle  && dotProd >= 0.0f && GameEngine::getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
		{
			if (!mainEngine.isRunning())
			{
				mainEngine.setContinuousSpred();
				mainEngine.launch();
			}
		}
		else if (mainEngine.isRunning())
			mainEngine.setSingleSpread();
	}
	obj = object.getChild(enginesNames[1]);
	if (obj->isUsable())
	{
		ParticleSystem& mainEngineSmoke = static_cast<ParticleSystem&>(*obj);
		if (angle < limitAngle && dotProd >= 0.0f && GameEngine::getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			if (!mainEngineSmoke.isRunning())
			{
				mainEngineSmoke.setContinuousSpred();
				mainEngineSmoke.launch();
			}
		}
		else if (mainEngineSmoke.isRunning())
			mainEngineSmoke.setSingleSpread();
	}
}

void EnginesControl::processManeuverEngine(int nameIdx, bool status)
{
	if (nameIdx >= 0 && nameIdx < enginesNames.size())
	{
		GameObjectPtr obj = object.getChild(enginesNames[nameIdx]);
		if (obj->isUsable())
		{
			ParticleSystem& engine = static_cast<ParticleSystem&>(*obj);

			if (status)
			{
				if (!engine.isRunning())
				{
					engine.setContinuousSpred();
					engine.launch();
				}
			}
			else if (engine.isRunning())
				engine.setSingleSpread();
		}
	}
}

void EnginesControl::invalidate()
{

}

bool EnginesControl::areEqual(const glm::vec3& vec1, const glm::vec3& vec2, float eps)
{
	for (int i = 0; i < 3; ++i)
	{
		if (std::abs(vec1[i] - vec2[i]) > eps)
			return false;
	}
	return true;
}

void registerSpaceshipEnginesControl(GameEngine& engine)
{
	engine.getResources()->spaceship->addProperty<EnginesControl>("engines_control");
}

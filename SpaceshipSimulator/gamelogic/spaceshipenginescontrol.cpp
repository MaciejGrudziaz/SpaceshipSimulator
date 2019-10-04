#include "spaceshipenginescontrol.h"

void EnginesControl::init()
{
	currRotSpeed = 0.0f;
	currSpeed = 0.0f;
	mainEngineThrust = 20000;
	maneuverEngineThrust = 100000;	//1000kN
	spaceshipMass = 1000; //1000kg

	ManeuverEnginePtr lowerRight = std::make_shared<ManeuverEngine>();
	lowerRight->thrust = maneuverEngineThrust;
	lowerRight->thrustForceDirection = glm::normalize(glm::vec3(-1.0f, 1.0f, 0.0f));
	lowerRight->forceArm = glm::vec3(0.0f, -3.0f, 0.0f);
	maneuverEngines.insert(ManeuverEngineMap::value_type(enginesNames[2], lowerRight));

	ManeuverEnginePtr lowerLeft = std::make_shared<ManeuverEngine>();
	lowerLeft->thrust = maneuverEngineThrust;
	lowerLeft->thrustForceDirection = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));
	lowerLeft->forceArm = glm::vec3(0.0f, -3.0f, 0.0f);
	maneuverEngines.insert(ManeuverEngineMap::value_type(enginesNames[3], lowerLeft));

	ManeuverEnginePtr upperLeft = std::make_shared<ManeuverEngine>();
	upperLeft->thrust = maneuverEngineThrust;
	upperLeft->thrustForceDirection = glm::normalize(glm::vec3(1.0f, -1.0f, 0.0f));
	upperLeft->forceArm = glm::vec3(0.0f, 3.0f, 0.0f);
	maneuverEngines.insert(ManeuverEngineMap::value_type(enginesNames[4], upperLeft));

	ManeuverEnginePtr upperRight = std::make_shared<ManeuverEngine>();
	upperRight->thrust = maneuverEngineThrust;
	upperRight->thrustForceDirection = glm::normalize(glm::vec3(-1.0f, -1.0f, 0.0f));
	upperRight->forceArm = glm::vec3(0.0f, 3.0f, 0.0f);
	maneuverEngines.insert(ManeuverEngineMap::value_type(enginesNames[5], upperRight));
}
void EnginesControl::process()
{
	processMainEngine();

	procesAllManeuverEngines();

	checkPosition();

	spaceshipMovement();
}

void EnginesControl::processMainEngine()
{
	static Spaceship& spaceship = static_cast<Spaceship&>(object);
	const glm::vec3 forwardDir(0.0f, 1.0f, 0.0f);
	const float limitAngle = 25.0f;	//degress

	glm::vec3 moveDir = spaceship.getInputMoveVec();
	float angle = glm::degrees(glm::acos(glm::dot(forwardDir, object.getTransform().getOrientation())));

	processMainEngineCentralFlame(moveDir, angle, limitAngle);
	processMainEngineExhaustCloud(moveDir, angle, limitAngle);
}

void EnginesControl::processMainEngineCentralFlame(const glm::vec3& moveDir, float angle, float limitAngle)
{
	static Spaceship& spaceship = static_cast<Spaceship&>(object);

	GameObjectPtr obj = object.getChild(enginesNames[0]);
	if (obj->isUsable())
	{
		ParticleSystem& mainEngine = static_cast<ParticleSystem&>(*obj);
		if (angle < limitAngle  && GameEngine::getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
		{
			if (!mainEngine.isRunning())
			{
				mainEngine.setContinuousSpred();
				mainEngine.launch();
			}

			float speed = spaceship.getWorldSpeed();
			speed += mainEngineThrust / spaceshipMass * Time::deltaTime;
			spaceship.setWorldSpeed(speed);
		}
		else if (mainEngine.isRunning())
		{
			mainEngine.setSingleSpread();

			float speed = spaceship.getWorldSpeed();
			speed -= mainEngineThrust / spaceshipMass * Time::deltaTime;
			if (speed < 0.0f) speed = 0.0f;
			spaceship.setWorldSpeed(speed);
		}
	}
}

void EnginesControl::processMainEngineExhaustCloud(const glm::vec3& moveDir, float angle, float limitAngle)
{
	static Spaceship& spaceship = static_cast<Spaceship&>(object);

	GameObjectPtr obj = object.getChild(enginesNames[1]);
	if (obj->isUsable())
	{
		ParticleSystem& mainEngineSmoke = static_cast<ParticleSystem&>(*obj);
		if (angle < limitAngle && GameEngine::getKey(GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS)
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

void EnginesControl::procesAllManeuverEngines()
{
	IdxPair launchMoveEngines = maneuverEnginesSpaceshipMovement();
	IdxPair launchRotEngines = maneuverEnginesSpaceshipRotation();

	processManeuverEngine(launchMoveEngines.first, true);
	processManeuverEngine(launchMoveEngines.second, true);
	processManeuverEngine(launchRotEngines.first, true);
	processManeuverEngine(launchRotEngines.second, true);

	for (int i = 2; i < enginesNames.size(); ++i)
	{
		if (!isInPair(i,launchMoveEngines) && !isInPair(i, launchRotEngines)) 
		{
			processManeuverEngine(i, false);
		}
	}

	updateManeuverEngines(launchMoveEngines, launchRotEngines);

	calcManeuverEnginesSpeedVec();
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

EnginesControl::IdxPair EnginesControl::maneuverEnginesSpaceshipMovement()
{
	static Spaceship& spaceship = static_cast<Spaceship&>(object);
	const float engineInfluenceAngle = 22.5f; //degrees

	IdxPair engines(-1, -1);
	glm::vec3 moveDir = spaceship.getInputMoveVec();

	if (!areEqual(moveDir, glm::vec3(0.0f)))
	{
		moveDir = glm::normalize(moveDir);

		for (int i = 0; i < dirVectors.size(); ++i)
		{
			glm::vec3 locDirVec = (object.getTransform().getRotationQuat() * dirVectors[i]);
			float angle = glm::degrees(glm::acos(glm::dot(locDirVec, moveDir)));

			if (angle < engineInfluenceAngle) {
				engines = directionEngines[i];
				break;
			}
		}
	}

	return engines;
}

EnginesControl::IdxPair EnginesControl::maneuverEnginesSpaceshipRotation()
{
	static Spaceship& spaceship = static_cast<Spaceship&>(object);

	IdxPair engines(-1, -1);
	float rot =  spaceship.getInputRotation();

	if (rot < 0.0f)
	{
		engines.first = 2;
		engines.second = 4;
	}
	else if(rot > 0.0f){
		engines.first = 3;
		engines.second = 5;
	}

	return engines;
}

void EnginesControl::updateManeuverEngines(IdxPair launchMoveEngines, IdxPair launchRotEngines)
{
	if (launchMoveEngines.first != -1)
		maneuverEngines.find(enginesNames[launchMoveEngines.first])->second->launch();
	if (launchMoveEngines.second != -1)
		maneuverEngines.find(enginesNames[launchMoveEngines.second])->second->launch();
	if (launchRotEngines.first != -1)
		maneuverEngines.find(enginesNames[launchRotEngines.first])->second->launch();
	if (launchRotEngines.second != -1)
		maneuverEngines.find(enginesNames[launchRotEngines.second])->second->launch();

	for (int i = 2; i < enginesNames.size(); ++i)
	{
		if (!isInPair(i, launchMoveEngines) && !isInPair(i, launchRotEngines))
		{
			maneuverEngines.find(enginesNames[i])->second->stop();
		}
	}
}

void EnginesControl::calcManeuverEnginesSpeedVec()
{
	glm::vec3 globalSpeed;

	glm::vec3 currEngineThrustVec;
	float forceMoment = 0.0f;
	for (auto engine : maneuverEngines)
	{
		if (engine.second->isActive)
		{
			currEngineThrustVec += engine.second->thrustForceDirection * engine.second->thrust;

			glm::vec3 crossProd = glm::cross(engine.second->forceArm, engine.second->thrustForceDirection);
			forceMoment += crossProd.z * engine.second->thrust;
		}
	}
	currEngineThrustVec = object.getTransform().getRotationQuat() * currEngineThrustVec;
	glm::vec3 acceleration = currEngineThrustVec / spaceshipMass;
	
	float currAngleAcceleration = forceMoment / spaceshipMass;

	//currRotSpeed += glm::cross(glm::vec3())
	currRotSpeed += currAngleAcceleration * Time::deltaTime;
	currSpeed += acceleration.x * Time::deltaTime;
}

void EnginesControl::spaceshipMovement()
{
	static Spaceship& spaceship = static_cast<Spaceship&>(object);

	glm::vec3 moveVec = spaceship.getInputMoveVec();
	float rotVal = spaceship.getInputRotation();

	glm::vec3 pos = object.getTransform().getPosition();
	pos += glm::vec3(currSpeed,0.0f,0.0f) * Time::deltaTime;
	object.getTransform().setPosition(pos);

	glm::vec3 rotation = object.getTransform().getRotation();
	rotation.z += currRotSpeed * Time::deltaTime;

	if (rotation.z > 360.0f) rotation.z = rotation.z - 360.0f;
	if (rotation.z < -360.0f) rotation.z = rotation.z + 360.0f;

	object.getTransform().setRotation(rotation);
}

void EnginesControl::checkPosition()
{
	static Spaceship& spaceship = static_cast<Spaceship&>(object);
	static int cnt = 0;
	++cnt;

	glm::vec3 globalPos = spaceship.getTransform().getPosition();
	glm::mat4 VP = (*spaceship.getProjectionMat()) * (*spaceship.getCamera()->getViewPtr());
	glm::vec4 clipSpacePos = (VP * glm::vec4(globalPos, 1.0f));
	clipSpacePos /= clipSpacePos.w;

	glm::vec3 moveDir = spaceship.getInputMoveVec();

	if (clipSpacePos.x >= 0.9f && currSpeed > 0.0f)
		currSpeed = 0.0f;
	if (clipSpacePos.x <= -0.9f && currSpeed < 0.0f)
		currSpeed = 0.0f;
}

void EnginesControl::invalidate()
{}

bool EnginesControl::areEqual(const glm::vec3& vec1, const glm::vec3& vec2, float eps)
{
	for (int i = 0; i < 3; ++i)
	{
		if (std::abs(vec1[i] - vec2[i]) > eps)
			return false;
	}
	return true;
}

bool EnginesControl::isInPair(int i, IdxPair indexes)
{
	if (i == indexes.first || i == indexes.second)
		return true;
	else return false;
}

void registerSpaceshipEnginesControl(GameEngine& engine)
{
	engine.getResources()->spaceship->addProperty<EnginesControl>("engines_control");
}

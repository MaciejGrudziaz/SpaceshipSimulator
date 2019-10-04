#pragma once

#include "../gameengine.h"
#include "../gameobjects/gameresources.h"

struct ManeuverEngine 
{
	float thrust;
	glm::vec3 thrustForceDirection;
	glm::vec3 forceArm;
	bool isActive;

	ManeuverEngine()
		: thrust(0.0f)
		, isActive(false)
	{}

	void launch()
	{
		isActive = true;
	}

	void stop()
	{
		isActive = false;
	}
};

typedef std::shared_ptr<ManeuverEngine> ManeuverEnginePtr;

class EnginesControl : public Property<GameObject>
{
public:
	EnginesControl(const std::string& name, GameObject& object)
		: Property<GameObject>(name, object)
	{}

	void init()override;
	void process()override;
	void invalidate()override;

private:
	const std::array<std::string, 6> enginesNames { "particles_mainEngine",  "particles_mainEngineSmoke",	
													"particles_lowerRightEngine", "particles_lowerLeftEngine", 
													"particles_upperLeftEngine", "particles_upperRightEngine" };
	const std::array<glm::vec3, 8> dirVectors{  glm::vec3(0.0f,1.0f,0.0f), glm::vec3(1.0f,0.0f,0.0f), glm::vec3(0.0f,-1.0f,0.0f), glm::vec3(-1.0f,0.0f,0.0f),
												glm::vec3(std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f, 0.0f), glm::vec3(std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f, 0.0f),
												glm::vec3(-std::sqrt(2.0f) / 2.0f, std::sqrt(2.0f) / 2.0f, 0.0f), glm::vec3(-std::sqrt(2.0f) / 2.0f, -std::sqrt(2.0f) / 2.0f, 0.0f) };

	typedef std::pair<int, int> IdxPair;
	const std::array<IdxPair, 8> directionEngines{ IdxPair(2,3), IdxPair(4,3), IdxPair(4, 5), IdxPair(2, 5), 
												   IdxPair(3,-1), IdxPair(4,-1), IdxPair(2, -1), IdxPair(5, -1) };
	
	typedef std::map<std::string, ManeuverEnginePtr> ManeuverEngineMap;
	ManeuverEngineMap maneuverEngines;

	float mainEngineThrust;
	float spaceshipMass;
	float maneuverEngineThrust;
	float currSpeed;
	float currRotSpeed;

	bool areEqual(const glm::vec3& vec1, const glm::vec3& vec2, float eps = 0.0000001f);
	void processMainEngine();
	void processMainEngineCentralFlame(const glm::vec3& moveDir, float angle, float limitAngle);
	void processMainEngineExhaustCloud(const glm::vec3& moveDir, float angle, float limitAngle);

	void procesAllManeuverEngines();
	void processManeuverEngine(int nameIdx, bool status);
	IdxPair maneuverEnginesSpaceshipMovement();
	IdxPair maneuverEnginesSpaceshipRotation();
	void updateManeuverEngines(IdxPair launchMoveEngines, IdxPair launchRotEngines);
	void calcManeuverEnginesSpeedVec();

	void checkPosition();

	bool isInPair(int i, IdxPair indexes);
	void spaceshipMovement();
};

void registerSpaceshipEnginesControl(GameEngine& engine);

#pragma once

#include "../gameengine.h"
#include "../gameobjects/gameresources.h"

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

	bool initalized;
	glm::vec3 prevPos;

	bool areEqual(const glm::vec3& vec1, const glm::vec3& vec2, float eps = 0.0000001f);
	void processMainEngine(bool val);
	void processManeuverEngine(int nameIdx, bool status);
};

void registerSpaceshipEnginesControl(GameEngine& engine);

#pragma once

#include "standardgameobject.h"
#include "../utilities/time.h"

class Asteroid: public StandardGameObject
{
public:
	Asteroid();

	//void init(const glm::vec3& position, const glm::vec3& rotSpeed, const glm::vec3& linSpeed, float scale);

	void process()override;

	void deepCopy(const Asteroid& asteroid);

	void registerWorldSpeed(std::shared_ptr<float> speed);

	void setRotSpeed(glm::vec3 rotSpeed);
	void setLinearSpeed(glm::vec3 linSpeed);

private:
	glm::vec3 rotSpeed;
	glm::vec3 linearSpeed;
	std::shared_ptr<float> worldSpeed;
};

typedef std::shared_ptr<Asteroid> AsteroidPtr;

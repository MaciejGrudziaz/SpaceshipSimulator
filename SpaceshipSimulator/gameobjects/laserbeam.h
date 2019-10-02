#pragma once

#include <glm/gtx/vector_angle.hpp>
#include <GameResources/gameobjects/gameobject.h>
#include <GameRenderer/uniformdata.h>
#include "../utilities/time.h"

class LaserBeam : public GameObject
{
public:
	LaserBeam();

	void process()override;

	void launch(const glm::vec3& startPos, const glm::vec3& direction);

	void loadViewMatrixPtr(ConstMat4Ptr view);
	void loadProjectionMatrixPtr(ConstMat4Ptr projection);

	void setSpeed(float speed);
	void setLength(float val);

	float getSpeed()const;
	float getLength()const;
	glm::vec3 getDirection()const;

private:
	ConstMat4Ptr viewPtr;
	ConstMat4Ptr projectionPtr;

	float speed;
	float length;
	glm::vec3 direction;
	bool launchFlag;
};

typedef std::shared_ptr<LaserBeam> LaserBeamPtr;

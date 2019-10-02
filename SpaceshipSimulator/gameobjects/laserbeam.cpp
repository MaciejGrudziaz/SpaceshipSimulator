#include "laserbeam.h"

LaserBeam::LaserBeam()
	: speed(1.0f)
	, length(1.0f)
	, direction(0.0f,0.0f,-1.0f)
	, launchFlag(false)
{
	//transform.setDefaultOrientation(glm::vec3(0.0f, 1.0f, 0.0f));
}

void LaserBeam::process()
{
	GameObject::process();

	if (launchFlag)
	{
		glm::vec3 pos = transform.getPosition();
		pos += speed * direction * Time::deltaTime;
		transform.setPosition(pos);
	}
}

void LaserBeam::launch(const glm::vec3& startPos, const glm::vec3& direction)
{
	transform.setPosition(startPos);
	this->direction = direction;
	launchFlag = true;
}

void LaserBeam::loadViewMatrixPtr(ConstMat4Ptr view)
{
	viewPtr = view;
}

void LaserBeam::loadProjectionMatrixPtr(ConstMat4Ptr projection)
{
	projectionPtr = projection;
}

void LaserBeam::setSpeed(float speed)
{
	this->speed = speed;
}

void LaserBeam::setLength(float val)
{
	length = val;
}

float LaserBeam::getSpeed()const
{
	return speed;
}

float LaserBeam::getLength()const
{
	return length;
}

glm::vec3 LaserBeam::getDirection()const
{
	return direction;
}

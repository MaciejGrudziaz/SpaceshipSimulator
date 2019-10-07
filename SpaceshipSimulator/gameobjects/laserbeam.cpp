#include "laserbeam.h"

LaserBeam::LaserBeam()
	: speed(1.0f)
	, length(1.0f)
	//, currPos(std::make_shared<glm::vec3>(0.0f))
{
	transform.setDefaultOrientation(glm::vec3(0.0f, 1.0f, 0.0f));
	setActive(false);
}

void LaserBeam::process()
{
	GameObject::process();

	if (isActive())
	{
		glm::vec3 pos = transform.getPosition();
		pos += speed * transform.getOrientation() * Time::deltaTime;
		transform.setPosition(pos);
		//*currPos = pos;
	}
}

void LaserBeam::launch(const glm::vec3& startPos, const glm::quat& rotation)
{
	transform.setPosition(startPos);
	transform.setRotation(rotation);
	setActive(true);
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

//std::shared_ptr<glm::vec3> LaserBeam::getCurrentPosPtr()const
//{
//	return currPos;
//}

#include <stdafx.h>

Camera::Camera()
	: view(std::make_shared<glm::mat4>(1.0f))
	, up(std::make_shared<glm::vec3>(0.0f, 1.0f, 0.0f))
	, right(std::make_shared<glm::vec3>(1.0f, 0.0f, 0.0f))
{}

Transform& Camera::getTransform()
{
	return transform;
}

glm::vec3 Camera::getUpVec()const
{
	return *up;
}

std::shared_ptr<glm::vec3> Camera::getUpVecPtr()const
{
	return up;
}

glm::vec3 Camera::getRightVec()const
{
	return *right;
}

std::shared_ptr<glm::vec3> Camera::getRightVecPtr()const
{
	return right;
}

glm::mat4 Camera::getView()const
{
	return *view;
}

std::shared_ptr<const glm::mat4> Camera::getViewPtr()const
{
	return view;
}

void Camera::update()
{
	glm::vec3 position = transform.getPosition();
	*view = glm::lookAt(position, position + transform.getOrientation(), glm::vec3(0.0f,1.0f,0.0f));
	*up = transform.getRotationQuat() * glm::vec3(0.0f, 1.0f, 0.0f);
	*right = transform.getRotationQuat() * glm::vec3(1.0f, 0.0f, 0.0f);
}
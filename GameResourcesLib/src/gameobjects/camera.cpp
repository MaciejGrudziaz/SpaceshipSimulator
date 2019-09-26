#include <stdafx.h>

Camera::Camera()
	: up(glm::vec3(0.0f,1.0f,0.0f))
	, view(std::make_shared<glm::mat4>(1.0f))
{}

Transform& Camera::getTransform()
{
	return transform;
}

void Camera::setUpVec(const glm::vec3& up)
{
	this->up = up;
}

glm::vec3 Camera::getUpVec()const
{
	return up;
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
	*view = glm::lookAt(position, position + transform.getOrientation(), up);
}
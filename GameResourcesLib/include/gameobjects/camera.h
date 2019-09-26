#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "transform.h"

class Camera
{
public:
	Camera();

	Transform& getTransform();

	void setUpVec(const glm::vec3& up);
	glm::vec3 getUpVec()const;

	glm::mat4 getView()const;
	std::shared_ptr<const glm::mat4> getViewPtr()const;

	virtual void update();

protected:
	
	Transform transform;

	glm::vec3 up;
	std::shared_ptr<glm::mat4> view;
};

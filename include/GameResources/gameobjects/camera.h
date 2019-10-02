#pragma once

#include <glm/vec3.hpp>
#include <glm/matrix.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include "transform.h"

class Camera
{
public:
	Camera();

	Transform& getTransform();

	//void setUpVec(const glm::vec3& up);
	glm::vec3 getUpVec()const;
	std::shared_ptr<glm::vec3> getUpVecPtr()const;

	glm::vec3 getRightVec()const;
	std::shared_ptr<glm::vec3> getRightVecPtr()const;

	glm::mat4 getView()const;
	std::shared_ptr<const glm::mat4> getViewPtr()const;

	virtual void update();

protected:
	
	Transform transform;

	//glm::vec3 up;
	std::shared_ptr<glm::mat4> view;
	std::shared_ptr<glm::vec3> up;
	std::shared_ptr<glm::vec3> right;
};

typedef std::shared_ptr<Camera> CameraPtr;

#include <stdafx.h>

Transform::Transform()
{
    initValues();
}

void Transform::initValues()
{
    position = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
	defaultOrientation = glm::vec3(0.0f, 0.0f, -1.0f);
	orientation = defaultOrientation;
    rotation = glm::vec3(0.0f);
    transform = glm::mat4(1.0f);
    rotationQuat = glm::quat();
    scaleMat = glm::mat4(1.0f);
}

glm::vec3 Transform::getPosition() const
{
    return position;
}

glm::vec3 Transform::getScale() const
{
    return scale;
}

glm::vec3 Transform::getDefaultOrientation()const
{
	return defaultOrientation;
}

glm::vec3 Transform::getOrientation() const
{
    return orientation;
}

glm::vec3 Transform::getRotation() const
{
    glm::vec3 degreeRotation = glm::degrees(rotation);

    return degreeRotation;
}

glm::mat4 Transform::getTransformMat() const
{
    return transform;
}

glm::quat Transform::getRotationQuat() const
{
    return rotationQuat;
}

glm::mat4 Transform::getScaleMat() const
{
    return scaleMat;
}

void Transform::setPosition(const glm::vec3& pos)
{
    position = pos;
    updateTransformMatrix();
}

void Transform::updateTransformMatrix()
{
    transform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotationQuat) * scaleMat;
}

void Transform::setScale(const glm::vec3& scale)
{
    this->scale = scale;
    updateScaleMatrix();
}

void Transform::updateScaleMatrix()
{
    scaleMat = glm::scale(glm::mat4(1.0f), scale);
    updateTransformMatrix();
}

void Transform::setScale(const glm::mat4& scale)
{
    scaleMat = scale;
    getScaleValuesFromScaleMatrix();
    updateTransformMatrix();
}

void Transform::getScaleValuesFromScaleMatrix()
{
    scale.x = scaleMat[0][0];
    scale.y = scaleMat[1][1];
    scale.z = scaleMat[2][2];
}

void Transform::setRotation(const glm::vec3& angles)
{
    rotation = glm::radians(angles);

    updateRotationQuat();
}

void Transform::updateRotationQuat()
{
    rotationQuat = glm::quat(rotation);
	updateOrientation();
    updateTransformMatrix();
}

void Transform::updateOrientation()
{
	orientation = rotationQuat * defaultOrientation;
}

void Transform::setRotation(const glm::quat& rot)
{
    rotationQuat = rot;
    getAnglesFromQuat();
	updateOrientation();
    updateTransformMatrix();
}

void Transform::getAnglesFromQuat()
{
    rotation = glm::eulerAngles(rotationQuat);
}

void Transform::setDefaultOrientation(const glm::vec3& orientation)
{
	defaultOrientation = glm::normalize(orientation);

	updateOrientation();
}

void Transform::reset()
{
    initValues();
}

void Transform::resetWithoutScale()
{
    glm::vec3 currScale = scale;

    initValues();

    scale = currScale;
    updateScaleMatrix();
}

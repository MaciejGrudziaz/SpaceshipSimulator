#include <stdafx.h>

ObjectTransform::ObjectTransform()
{
    initValues();
}

void ObjectTransform::initValues()
{
    position = glm::vec3(0.0f);
    scale = glm::vec3(1.0f);
    orientation = glm::vec3(0.0f,0.0f,-1.0f);
    rotation = glm::vec3(0.0f);
    transform = glm::mat4(1.0f);
    rotationQuat = glm::quat();
    scaleMat = glm::mat4(1.0f);
}

glm::vec3 ObjectTransform::getPosition() const
{
    return position;
}

glm::vec3 ObjectTransform::getScale() const
{
    return scale;
}

glm::vec3 ObjectTransform::getOrientation() const
{
    return orientation;
}

glm::vec3 ObjectTransform::getRotation() const
{
    glm::vec3 degreeRotation = glm::degrees(rotation);

    return degreeRotation;
}

glm::mat4 ObjectTransform::getTransformMat() const
{
    return transform;
}

glm::quat ObjectTransform::getRotationQuat() const
{
    return rotationQuat;
}

glm::mat4 ObjectTransform::getScaleMat() const
{
    return scaleMat;
}

void ObjectTransform::setPosition(const glm::vec3& pos)
{
    position = pos;
    updateTransformMatrix();
}

void ObjectTransform::updateTransformMatrix()
{
    transform = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(rotationQuat) * scaleMat;
}

void ObjectTransform::setScale(const glm::vec3& scale)
{
    this->scale = scale;
    updateScaleMatrix();
}

void ObjectTransform::updateScaleMatrix()
{
    scaleMat = glm::scale(glm::mat4(1.0f), scale);
    updateTransformMatrix();
}

void ObjectTransform::setScale(const glm::mat4& scale)
{
    scaleMat = scale;
    getScaleValuesFromScaleMatrix();
    updateTransformMatrix();
}

void ObjectTransform::getScaleValuesFromScaleMatrix()
{
    scale.x = scaleMat[0][0];
    scale.y = scaleMat[1][1];
    scale.z = scaleMat[2][2];
}

void ObjectTransform::setRotation(const glm::vec3& angles)
{
    rotation = glm::radians(angles);

    updateRotationQuat();
}

void ObjectTransform::updateRotationQuat()
{
    rotationQuat = glm::quat(rotation);
    updateTransformMatrix();
}

void ObjectTransform::setRotation(const glm::quat& rot)
{
    rotationQuat = rot;
    getAnglesFromQuat();
    updateTransformMatrix();
}

void ObjectTransform::getAnglesFromQuat()
{
    rotation = glm::eulerAngles(rotationQuat);
}

void ObjectTransform::setOrientation(const glm::vec3& vec)
{
    orientation = glm::normalize(vec);
}

void ObjectTransform::reset()
{
    initValues();
}

void ObjectTransform::resetWithoutScale()
{
    glm::vec3 currScale = scale;

    initValues();

    scale = currScale;
    updateScaleMatrix();
}

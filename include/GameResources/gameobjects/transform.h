#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <glm/matrix.hpp>
#include <glm/vec3.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Transform
{
public:
    Transform();

    glm::vec3 getPosition() const;
    glm::vec3 getScale() const;
    glm::vec3 getOrientation() const;
    glm::vec3 getRotation() const;      //euler angles in degrees

    glm::mat4 getTransformMat() const;
    glm::quat getRotationQuat() const;
    glm::mat4 getScaleMat() const;

    void setPosition(const glm::vec3& pos);
    void setScale(const glm::vec3& scale);
    void setScale(const glm::mat4& scale);
    void setRotation(const glm::vec3& angles); //euler angles in degress
    void setRotation(const glm::quat& rot);
    void setOrientation(const glm::vec3& vec);

    void reset();
    void resetWithoutScale();

private:
    glm::vec3 position;
    glm::vec3 scale;
    glm::vec3 orientation;
    glm::vec3 rotation;                 //euler angles in radians

    glm::mat4 transform;                //glm::mat4 is column major
    glm::quat rotationQuat;
    glm::mat4 scaleMat;

    void updateTransformMatrix();
    void updateRotationQuat();
    void updateScaleMatrix();
    void getAnglesFromQuat();
    void getScaleValuesFromScaleMatrix();

    void initValues();
};

#endif // TRANSFORM_H

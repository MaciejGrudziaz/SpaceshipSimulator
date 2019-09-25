#ifndef BASICOBJECT_H
#define BASICOBJECT_H

#include "basicobjectparts.h"
#include <glm/glm.hpp>
#include <vector>
#include <string>

class BasicObject {
public:
	glm::mat4 globalTransform;

	std::vector<ControlPoint> controlPoints;

	std::vector<Face> faces;

	std::string name;
	std::string textureName;

    std::shared_ptr<Skeleton> skeleton;

	std::vector<AnimationInfo> animationsInfo;

    BasicObject(){
        skeleton = std::make_shared<Skeleton>();
    }
    BasicObject(const BasicObject& object);

private:
    void copySkeleton(const BasicObject& object);
};

typedef std::shared_ptr<BasicObject> BasicObjectPtr;
typedef std::shared_ptr<Joint> BasicObjectJointPtr;
typedef std::shared_ptr<AnimationJoint> BasicObjectAnimationJointPtr;
typedef std::shared_ptr<Keyframe> BasicObjectKeyframePtr;
typedef std::shared_ptr<Skeleton> BasicObjectSkeletonPtr;

#endif //BASICOBJECT_H

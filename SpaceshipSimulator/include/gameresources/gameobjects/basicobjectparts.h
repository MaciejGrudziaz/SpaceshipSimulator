#ifndef BASICOBJECTPARTS_H
#define BASICOBJECTPARTS_H

#include <glm/matrix.hpp>
#include <string>
#include <vector>

struct Face {
    int indices[3];
    glm::vec4 normals[3];
    glm::vec2 uv[3];
};

struct Keyframe {
    int frameNum;
    glm::mat4 globalTransform;
    std::weak_ptr<Keyframe> nextFrame;
};

struct AnimationInfo {
    std::string name;
    int frameCount;
};

struct AnimationJoint {
    std::string name;
    int frameCount;
    std::vector<std::shared_ptr<Keyframe> > frames;
    AnimationJoint():frameCount(0) {}
    ~AnimationJoint() {}
};

struct Joint {
    int parentIndex;
    std::string name;

    glm::mat4 globalBindposeInverse;

    glm::vec4 bindPos;

    glm::mat4 transform;

    std::vector<std::shared_ptr<AnimationJoint> > animations;

    Joint() :parentIndex(-1),transform(1.0f) {}

    ~Joint() {}
};

struct Skeleton {
    int FindJointIndexUsingName(std::string jointName) {
        for (unsigned i = 0; i < joints.size(); ++i) {
            if (joints[i]->name == jointName) return i;
        }
        return -1;
    }

    std::vector<std::shared_ptr<Joint> > joints;

    ~Skeleton() {}
};


struct BlendingIndexWeightPair {
    float jointIndex;
    float weight;
};

struct ControlPoint {
    glm::vec3 coords;
    std::vector<BlendingIndexWeightPair> blendingInfo;
};

#endif // BASICOBJECTPARTS_H

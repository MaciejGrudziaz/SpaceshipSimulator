#include <stdafx.h>

BasicObject::BasicObject(const BasicObject& object)
{
    this->globalTransform = object.globalTransform;
    this->name = object.name;
    this->textureName = object.textureName;

    for(ControlPoint pt:object.controlPoints)
        this->controlPoints.push_back(pt);

    for(Face face:object.faces){
        Face newFace;
        std::memcpy(newFace.indices,face.indices,3*sizeof(int));
        std::memcpy(newFace.normals,face.normals,3*sizeof(glm::vec4));
        std::memcpy(newFace.uv,face.uv,2*sizeof(glm::vec2));
    }

    copySkeleton(object);

    for(AnimationInfo info:object.animationsInfo)
        this->animationsInfo.push_back(info);

}

void BasicObject::copySkeleton(const BasicObject& object)
{
    this->skeleton = std::make_shared<Skeleton>();
    for(std::shared_ptr<Joint> joint:object.skeleton->joints){
        std::shared_ptr<Joint> newJoint = std::make_shared<Joint>();
        newJoint->parentIndex = joint->parentIndex;
        newJoint->name = joint->name;
        newJoint->globalBindposeInverse = joint->globalBindposeInverse;
        newJoint->bindPos = joint->bindPos;

        for(std::shared_ptr<AnimationJoint> animJoint : joint->animations){
            std::shared_ptr<AnimationJoint> newAnimJoint = std::make_shared<AnimationJoint>();
            newAnimJoint->name = animJoint->name;
            newAnimJoint->frameCount = animJoint->frameCount;

            for(int i=0;i<animJoint->frames.size();++i){
                std::shared_ptr<Keyframe> frame = animJoint->frames[i];
                std::shared_ptr<Keyframe> newFrame = std::make_shared<Keyframe>();
                newFrame->frameNum = frame->frameNum;
                newFrame->globalTransform = frame->globalTransform;
                if(i>0) newAnimJoint->frames[i-1]->nextFrame=newFrame;

                newAnimJoint->frames.push_back(newFrame);
            }

            newJoint->animations.push_back(newAnimJoint);
        }

        this->skeleton->joints.push_back(newJoint);
    }
}

#include "mgrimportjoint.h"

int MgrImportJoint::loadJoint(const glm::mat4& objectGlobalTransform, std::fstream& file) {
    joint = std::make_shared<Joint>();

    loadJointName(file);

    if(!errorFlag) loadJointParentIndex(file);

    if(!errorFlag) loadJointGlobalBindposeMatrix(objectGlobalTransform,file);

    if(!errorFlag) loadJointsAnimations(file);

    return errorFlag;
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void MgrImportJoint::loadJointName(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));
    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    joint->name = std::string(block);

    errorFlag = 0;
}

void MgrImportJoint::loadJointParentIndex(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));
    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    joint->parentIndex = *((int*)block);

    errorFlag = 0;
}

void MgrImportJoint::loadJointGlobalBindposeMatrix(const glm::mat4& objectGlobalTransformMat, std::fstream& file)
{
    Mat4Struct globalBindposeInverse;

    file.read((char*)(&globalBindposeInverse), sizeof(Mat4Struct));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    joint->globalBindposeInverse = globalBindposeInverse.getGlmMat();

    joint->bindPos = objectGlobalTransformMat * glm::vec4((glm::inverse(joint->globalBindposeInverse))[3]);

    errorFlag = 0;
}

void MgrImportJoint::loadJointsAnimations(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));

    while(!errorFlag){
        file.read(block, blockSize * sizeof(char));

        loadJointAnimationData(file,block);

        if(errorFlag==2) checkJointSectionEnd(file,block);
    }

    if(errorFlag==3) errorFlag = 0;
}

void MgrImportJoint::loadJointAnimationData(std::fstream& file, const char* block)
{
    if (strcmp(block, FILEHDR::animationsHeader) == 0) {
        std::unique_ptr<MgrImportAnimation> anim = std::make_unique<MgrImportAnimation>(blockSize);
        if (anim->loadAnimation(file) != 0) {
            errorFlag = 1;
            return;
        }
        joint->animations.push_back(anim->getAnimationJoint());
        errorFlag = 0;
        return;
    }

    errorFlag = 2;
}

void MgrImportJoint::checkJointSectionEnd(std::fstream& file, const char* block)
{
    if (strcmp(block, FILEHDR::jointFooter)==0) {
        errorFlag = 3;
        return;
    }
    else if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }

    errorFlag = 0;
}

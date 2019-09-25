#include "mgrimportskeleton.h"

int MgrImportSkeleton::loadSkeleton(const glm::mat4& objectGlobaltransform, std::fstream& file) {
    //char block[blockSize] = { 0 };
    std::memset(block,0x00,blockSize*sizeof(char));

    skeleton = std::make_shared<Skeleton>();

    do {
        file.read(block, blockSize * sizeof(char));

        loadSkeletonJoint(objectGlobaltransform, file,block);

        if(errorFlag==2) checkSkeletonSectionEnd(file,block);
    } while (!errorFlag);

    if(errorFlag==3) errorFlag = 0;
    return errorFlag;
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void MgrImportSkeleton::loadSkeletonJoint(const glm::mat4& objectGlobaltransform, std::fstream& file,const char* block)
{
    if (strcmp(block, FILEHDR::jointHeader) == 0) {
        std::unique_ptr<MgrImportJoint> joint = std::make_unique<MgrImportJoint>(blockSize);
        if (joint->loadJoint(objectGlobaltransform, file) != 0){
            errorFlag = 1;
            return;
        }
        skeleton->joints.push_back(joint->getJoint());
        errorFlag = 0;
        return;
    }
    errorFlag = 2;
}

void MgrImportSkeleton::checkSkeletonSectionEnd(std::fstream& file,const char* block)
{
    if (strcmp(block, FILEHDR::skeletonFooter)==0){
        errorFlag = 3;
        return;
    }
    else if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    errorFlag = 0;
}

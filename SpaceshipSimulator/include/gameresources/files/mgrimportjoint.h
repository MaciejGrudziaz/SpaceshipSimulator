#ifndef MGRIMPORTJOINT_H
#define MGRIMPORTJOINT_H

#include "exportimportstruct.h"
#include "../gameobjects/basicobject.h"
#include "mgrimportanimation.h"

class MgrImportJoint: public MgrImport
{
    BasicObjectJointPtr joint;

    void loadJointName(std::fstream& file);
    void loadJointParentIndex(std::fstream& file);
    void loadJointGlobalBindposeMatrix(const glm::mat4& objectGlobalTransformMat, std::fstream& file);
    void loadJointsAnimations(std::fstream& file);
    void loadJointAnimationData(std::fstream& file, const char* block);
    void checkJointSectionEnd(std::fstream& file, const char* block);

public:
    MgrImportJoint(int blockSize): MgrImport(blockSize) { }
    ~MgrImportJoint() { }

    int loadJoint(const glm::mat4& objectGlobalTransform, std::fstream& file);

    BasicObjectJointPtr getJoint()const { return joint; }
};

#endif // IMPORTJOINT_H

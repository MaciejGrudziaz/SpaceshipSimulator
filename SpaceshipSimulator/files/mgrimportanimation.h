#ifndef MGRIMPORTANIMATION_H
#define MGRIMPORTANIMATION_H

#include <fstream>
#include "exportimportstruct.h"
#include "../resources/basicobject.h"
#include "mgrimportanimationframe.h"

class MgrImportAnimation: public MgrImport
{
    BasicObjectAnimationJointPtr animationJoint;

    void loadAnimationName(std::fstream& file);
    void loadAnimationFrameCount(std::fstream& file);
    void loadAnimationFrames(std::fstream& file);
    void loadAnimationFrameData(std::fstream& file, const char* headerBlock);
    void checkAnimationSectionEnd(std::fstream& file, const char* footerBlock);

public:
    MgrImportAnimation(int blockSize): MgrImport(blockSize) { }
    ~MgrImportAnimation() { }

    int loadAnimation(std::fstream& file);

    BasicObjectAnimationJointPtr getAnimationJoint()const { return animationJoint; }
};

#endif // MGRIMPORTANIMATION_H

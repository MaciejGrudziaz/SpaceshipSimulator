#ifndef MGRIMPORTSKELETON_H
#define MGRIMPORTSKELETON_H

#include <fstream>
#include "exportimportstruct.h"
#include "../resources/basicobject.h"
#include "mgrimportjoint.h"

class MgrImportSkeleton: public MgrImport
{
    BasicObjectSkeletonPtr skeleton;

    void loadSkeletonJoint(const glm::mat4& objectGlobaltransform, std::fstream& file, const char* block);
    void checkSkeletonSectionEnd(std::fstream& file, const char* block);

public:
    MgrImportSkeleton(int blockSize): MgrImport(blockSize) { }
    ~MgrImportSkeleton() { }

    int loadSkeleton(const glm::mat4& objectGlobaltransform, std::fstream& file);

    BasicObjectSkeletonPtr getSkeleton()const { return skeleton; }
};

#endif // MGRIMPORTSKELETON_H

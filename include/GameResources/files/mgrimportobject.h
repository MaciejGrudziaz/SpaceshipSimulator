#ifndef MGRIMPORTOBJECT_H
#define MGRIMPORTOBJECT_H

#include <fstream>
#include "mgrimport.h"
#include "exportimportstruct.h"
#include "../gameobjects/basicobject.h"
#include "mgrimportcontrolpoints.h"
#include "mgrimportfaces.h"
#include "mgrimportskeleton.h"

class MgrImportObject: public MgrImport
{
    BasicObjectPtr object;

    void loadObjectNames(std::fstream& file);
    void loadObjectGlobalTransformMat(std::fstream& file);
    void loadObjectControlPt(std::fstream& file);
    void loadObjectFaces(std::fstream& file);
    void loadObjectSkeleton(std::fstream& file);
    void checkObjectEnd(std::fstream& file);

public:
    MgrImportObject(int blockSize): MgrImport(blockSize) { }

    int loadObject(std::fstream& file);

    BasicObjectPtr getObject()const { return object; }
};

#endif // MGRIMPORTOBJECT_H

#ifndef MGRIMPORTCONTROLPOINTS_H
#define MGRIMPORTCONTROLPOINTS_H

#include <fstream>
#include "mgrimport.h"
#include "exportimportstruct.h"
#include "../resources/basicobject.h"

class MgrImportControlPoints: public MgrImport
{
    std::shared_ptr<std::vector<ControlPoint> > controlPoints;

    std::pair<int,int> loadControlPtCount(std::fstream& file);
    int loadControlPtData(std::shared_ptr<ControlPtStruct[]> controlPtIn, int controlPtCount, std::fstream& file);
    void loadControlPtToObject(std::shared_ptr<ControlPtStruct[]> controlPtIn, int controlPtCount);
    void checkControlPtSectionEnd(int bytesRead, std::fstream& file);

public:
    MgrImportControlPoints(int blockSize): MgrImport(blockSize) { }

    int loadControlPt(std::fstream& file);

    std::shared_ptr<std::vector<ControlPoint> > getControlPt()const { return controlPoints; }
};

#endif // IMPORTCONTROLPOINTS_H

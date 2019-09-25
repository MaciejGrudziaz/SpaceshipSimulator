//#include "mgrimportcontrolpoints.h"
#include <stdafx.h>

int MgrImportControlPoints::loadControlPt(std::fstream& file) {
    int controlPtCount = 0;
    int bytesRead = 0;

    controlPoints = std::make_shared<std::vector<ControlPoint> >();

    std::pair<int,int> ptCountVal = loadControlPtCount(file);
    controlPtCount = ptCountVal.first;
    bytesRead += ptCountVal.second;

    std::shared_ptr<ControlPtStruct[]> controlPtIn(new ControlPtStruct[controlPtCount]);

    if(!errorFlag) bytesRead += loadControlPtData(controlPtIn, controlPtCount, file);

    if(!errorFlag) loadControlPtToObject(controlPtIn, controlPtCount);

    if(!errorFlag) checkControlPtSectionEnd(bytesRead, file);

    return errorFlag;
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

std::pair<int,int> MgrImportControlPoints::loadControlPtCount(std::fstream& file)
{
    int controlPtCount;
    std::pair<int,int> outVal;

    file.read((char*)(&controlPtCount), sizeof(int));
    if ((file.rdstate()&std::fstream::eofbit) != 0)
        errorFlag = 1;
    else errorFlag = 0;

    outVal.first = controlPtCount;
    outVal.second = sizeof(int);

    return outVal;
}

int MgrImportControlPoints::loadControlPtData(std::shared_ptr<ControlPtStruct[]> controlPtIn, int controlPtCount, std::fstream& file)
{
    file.read((char*)controlPtIn.get(), controlPtCount * sizeof(ControlPtStruct));
    if ((file.rdstate()&std::fstream::eofbit) != 0)
        errorFlag = 1;
    else errorFlag = 0;

    return (controlPtCount * sizeof(ControlPtStruct));
}

void MgrImportControlPoints::loadControlPtToObject(std::shared_ptr<ControlPtStruct[]> controlPtIn, int controlPtCount)
{
    ControlPoint controlPt;

    for (int i = 0; i < controlPtCount; ++i) {
        controlPt.coords.x = controlPtIn[i].coords[0];
        controlPt.coords.y = controlPtIn[i].coords[1];
        controlPt.coords.z = controlPtIn[i].coords[2];

        for (int j = 0; j < 4; ++j) {
            BlendingIndexWeightPair blendAndWeight;
            blendAndWeight.jointIndex = controlPtIn[i].blendingJointIdx[j];
            blendAndWeight.weight = controlPtIn[i].blendingWeight[j];
            controlPt.blendingInfo.push_back(blendAndWeight);
        }

        controlPoints->push_back(controlPt);

        controlPt.blendingInfo.clear();
    }
}

void MgrImportControlPoints::checkControlPtSectionEnd(int bytesRead, std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));
    int blockAlignement = blockSize - (bytesRead%blockSize);

    file.read(block, blockAlignement * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }
    if (strcmp(block, FILEHDR::controlPtFooter) != 0){
        errorFlag = 1;
        return;
    }

    errorFlag = 0;
}

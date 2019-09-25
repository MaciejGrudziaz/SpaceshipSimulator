//#include "mgrimportobject.h"
#include <stdafx.h>

int MgrImportObject::loadObject(std::fstream& file) {
    std::memset(block,0x00,blockSize*sizeof(char));

    object = std::make_shared<BasicObject>();

    loadObjectNames(file);
    if(!errorFlag) loadObjectGlobalTransformMat(file);

    while(!errorFlag){
        file.read(block, blockSize * sizeof(char));

        loadObjectControlPt(file);
        if(errorFlag==2) loadObjectFaces(file);
        if(errorFlag==2) loadObjectSkeleton(file);
        if(errorFlag==2) checkObjectEnd(file);
    }

    if(errorFlag==3) return 0;

    return errorFlag;
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void MgrImportObject::loadObjectNames(std::fstream& file)
{
    //char block[blockSize] = { 0 };
    std::memset(block,0x00,blockSize*sizeof(char));

    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    object->name = std::string(block);
    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    object->textureName = std::string(block);

    errorFlag = 0;
}

void MgrImportObject::loadObjectGlobalTransformMat(std::fstream& file)
{
    Mat4Struct globalTransform;

    file.read((char*)(&globalTransform), sizeof(Mat4Struct));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    object->globalTransform = globalTransform.getGlmMat();

    errorFlag = 0;
}

void MgrImportObject::loadObjectControlPt(std::fstream& file)
{
    if (strcmp(block, FILEHDR::controlPtHeader) == 0) {
        std::unique_ptr<MgrImportControlPoints> controlPt=std::make_unique<MgrImportControlPoints>(blockSize);
        if (controlPt->loadControlPt(file) != 0) {
            errorFlag = 1;
            return;
        }

        std::shared_ptr<std::vector<ControlPoint> > importedControlPt = controlPt->getControlPt();
        for(ControlPoint pt : *importedControlPt)
            object->controlPoints.push_back(pt);

        errorFlag = 0;
        return;
    }
    errorFlag = 2;
}

void MgrImportObject::loadObjectFaces(std::fstream& file)
{
    if (strcmp(block, FILEHDR::facesHeader) == 0) {
        std::unique_ptr<MgrImportFaces> faces = std::make_unique<MgrImportFaces>(blockSize);
        if (faces->loadFaces(file) != 0) {
            errorFlag = 1;
            return;
        }

        std::shared_ptr<std::vector<Face> > importedFaces = faces->getFaces();
        for(Face face : *importedFaces)
            object->faces.push_back(face);

        errorFlag = 0;
        return;
    }
    errorFlag = 2;
}

void MgrImportObject::loadObjectSkeleton(std::fstream& file)
{
    if (strcmp(block, FILEHDR::skeletonHeader) == 0) {
        std::unique_ptr<MgrImportSkeleton> skeleton = std::make_unique<MgrImportSkeleton>(blockSize);
        if (skeleton->loadSkeleton(object->globalTransform, file) != 0) {
            errorFlag = 1;
            return;
        }

        object->skeleton = skeleton->getSkeleton();
        errorFlag = 0;
        return;
    }
    errorFlag = 2;
}

void MgrImportObject::checkObjectEnd(std::fstream& file)
{
    if (strcmp(block, FILEHDR::objectFooter) == 0) {
        errorFlag = 3;
        return;
    }
    else if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }
    errorFlag = 0;
}

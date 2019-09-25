//#include "mgrimportfaces.h"
#include <stdafx.h>

int MgrImportFaces::loadFaces(std::fstream& file) {
    int bytesRead = 0;
    int faceCount = 0;

    faces = std::make_shared<std::vector<Face> >();

    std::pair<int,int> faceCountVal = loadFacesCount(file);
    faceCount = faceCountVal.first;
    bytesRead += faceCountVal.second;

    //FaceStruct* faceIn = new FaceStruct[faceCount];
    std::shared_ptr<FaceStruct[]> faceIn(new FaceStruct[faceCount]);

    if(!errorFlag) bytesRead += loadFacesData(faceIn, faceCount, file);

    if(!errorFlag) loadFacesToObject(faceIn, faceCount);

    //delete[]faceIn;

    if(!errorFlag) checkFacesSectionEnd(bytesRead,file);

    return errorFlag;
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

std::pair<int,int> MgrImportFaces::loadFacesCount(std::fstream& file)
{
    int faceCount=0,bytesRead=0;
    std::pair<int,int> outVal;

    file.read((char*)(&faceCount), sizeof(int));
    if ((file.rdstate()&std::fstream::eofbit) != 0)
        errorFlag = 1;
    else errorFlag = 0;

    outVal.first = faceCount;
    outVal.second = sizeof(int);

    return outVal;
}

int MgrImportFaces::loadFacesData(std::shared_ptr<FaceStruct[]> faceIn, int faceCount, std::fstream& file)
{
    file.read((char*)faceIn.get(), faceCount * sizeof(FaceStruct));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        //delete[]faceIn;
        errorFlag = 1;
    }
    else errorFlag = 0;

    return (faceCount * sizeof(FaceStruct));
}

void MgrImportFaces::loadFacesToObject(std::shared_ptr<FaceStruct[]> faceIn, int faceCount)
{
    Face face;

    for (int i = 0; i < faceCount; ++i) {
        for (int j = 0; j < 3; ++j) {
            face.indices[j] = faceIn[i].indices[j];

            face.normals[j].x = faceIn[i].normal[j][0];
            face.normals[j].y = faceIn[i].normal[j][1];
            face.normals[j].z = faceIn[i].normal[j][2];
            face.normals[j].w = 0.0f;

            face.uv[j].x = faceIn[i].uv[j][0];
            face.uv[j].y = faceIn[i].uv[j][1];
        }

        //object->faces.push_back(face);
        faces->push_back(face);
    }
}

void MgrImportFaces::checkFacesSectionEnd(int bytesRead, std::fstream& file)
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
    if (strcmp(block, FILEHDR::facesFooter) != 0){
        errorFlag = 1;
        return;
    }

    errorFlag = 0;
}

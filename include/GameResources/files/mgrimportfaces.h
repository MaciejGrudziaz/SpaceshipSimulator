#ifndef MGRIMPORTFACES_H
#define MGRIMPORTFACES_H

#include <fstream>
#include "mgrimport.h"
#include "exportimportstruct.h"
#include "../gameobjects/basicobject.h"

class MgrImportFaces: public MgrImport
{
    std::shared_ptr<std::vector<Face> > faces;

    std::pair<int,int> loadFacesCount(std::fstream& file);
    int loadFacesData(std::shared_ptr<FaceStruct[]> faceIn, int faceCount, std::fstream& file);
    void loadFacesToObject(std::shared_ptr<FaceStruct[]> faceIn, int faceCount);
    void checkFacesSectionEnd(int bytesRead, std::fstream& file);

public:
    MgrImportFaces(int blockSize): MgrImport(blockSize) { }
    ~MgrImportFaces() { }

    int loadFaces(std::fstream& file);

    std::shared_ptr<std::vector<Face> > getFaces()const { return faces; }
};

#endif // IMPORTFACES_H

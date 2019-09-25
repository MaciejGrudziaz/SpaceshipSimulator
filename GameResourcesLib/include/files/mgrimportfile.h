#ifndef MGRIMPORTFILE_H
#define MGRIMPORTFILE_H

#include <fstream>
#include "exportimportstruct.h"
#include "../gameobjects/basicmodel.h"
#include "mgrimporthitboxes.h"
#include "mgrimportobject.h"

class MgrImportFile: public MgrImport {
    static const int baseBlockSize = 32;

    void checkFile(std::string filename);
    void checkHeader(std::fstream& file);
    void checkFooter(std::fstream& file);

    void loadScaleMat(std::fstream& file);
    void loadAllModels(std::fstream& file);
    void errorHandler(std::fstream& file);
    void loadModelDescriptionData();
    void loadAnimationsInfo(int index);
    void loadHitboxesNames(int index);

    void loadModel(std::fstream& file);
    int loadObjectData(std::fstream& file, int objIdx);
    void loadHitboxData(std::fstream& file, int objIdx);
    void loadImportedHitboxesToBasicModel();
    void checkFileEnd(std::fstream& file);

    //void transformImportedHitboxesToMap();

    glm::mat4 importedScaleMat;

    BasicModelPtr model;
    MgrImportHitboxesPtr importedHitboxes;

public:
    MgrImportFile():MgrImport(baseBlockSize),importedScaleMat(1.0f) { }
    ~MgrImportFile() { }

    int import(std::string filename);

    //ImportHitboxesPtr getHitboxes()const { return importedHitboxes; }
    //HitboxPtr getMainHitbox(int objectIdx);
    //HitboxMapPtr getHitboxes(int objectIdx);

    BasicModelPtr getModel() { return model; }

    int getErrorFlag() { return errorFlag; }

    glm::mat4 getScaleMat() { return importedScaleMat; }
};

typedef std::shared_ptr<MgrImportFile> MgrImportFilePtr;

#endif //MGRIMPORTFILE_H

#include "mgrimportfile.h"

int MgrImportFile::import(std::string filename) {
    checkFile(filename);

    if (errorFlag != 0) return errorFlag;

    model = std::make_shared<BasicModel>();
    importedHitboxes = std::make_shared<MgrImportHitboxes>(blockSize);

	std::fstream file(filename, std::ios::in | std::ios::binary);

    loadScaleMat(file);

    loadAllModels(file);

    loadModelDescriptionData();

    loadImportedHitboxesToBasicModel();

	return errorFlag;
}

HitboxPtr MgrImportFile::getMainHitbox(int objectIdx)
{
    if(objectIdx >=0 && objectIdx < model->getObjectsCount())
        return model->getMainHitbox(objectIdx);
}

HitboxMapPtr MgrImportFile::getHitboxes(int objectIdx)
{
    if(objectIdx>=0 && objectIdx < model->getObjectsCount())
        return model->getHitboxes(objectIdx);
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void MgrImportFile::checkFile(std::string filename) {
	std::fstream file(filename, std::ios::in | std::ios::binary);
	
    if (!file.is_open()){
        errorFlag = 2;
        return;
    }

    checkHeader(file);

    if(!errorFlag) checkFooter(file);

	file.close();
}

void MgrImportFile::checkHeader(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));

    file.read(block, blockSize * sizeof(char));

    if ((file.rdstate() & std::fstream::eofbit) != 0) {
        file.close();
        errorFlag = 1;
        return;
    }
    else {
        if (strcmp(block, FILEHDR::header) != 0) {
            file.close();
            errorFlag = 1;
            return;
        }
    }

    errorFlag = 0;
}

void MgrImportFile::checkFooter(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));

    do {
        file.read(block, blockSize * sizeof(char));
    } while ((file.rdstate()&std::fstream::eofbit) == 0);

    if (strncmp(block + (blockSize - (sizeof(FILEHDR::footer) / sizeof(char))), FILEHDR::footer,sizeof(FILEHDR::footer)/sizeof(char)) != 0) {
        file.close();
        errorFlag = 1;
        return;
    }

    errorFlag = 0;
}

void MgrImportFile::loadScaleMat(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));
    file.read(block, blockSize * sizeof(char));             //header
    Mat4Struct scaleMat;
    file.read((char*)scaleMat.mat, sizeof(Mat4Struct));     //scale matrix
    importedScaleMat = scaleMat.getGlmMat();
}

void MgrImportFile::loadAllModels(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));
    do {                                                    //model data
        file.read(block, blockSize * sizeof(char));

        if (strcmp(block, FILEHDR::modelHeader) == 0)
        {
            loadModel(file);
            if(errorFlag != 0)
                errorHandler(file);
        }

    } while ((file.rdstate() & std::fstream::eofbit) == 0 && !errorFlag);
}

void MgrImportFile::errorHandler(std::fstream& file)
{
    errorFlag = 5;                              //5 - error handler
    file.close();
}

void MgrImportFile::loadModelDescriptionData()
{
    if (errorFlag == 0) {
        for (int i = 0; i < model->getObjectsCount(); ++i) {
            loadAnimationsInfo(i);
            loadHitboxesNames(i);
        }
    }
}

void MgrImportFile::loadAnimationsInfo(int index)
{
    if (model->getObject(index)->skeleton->joints.size() > 0) {
        for (int j = 0; j < model->getObject(index)->skeleton->joints[0]->animations.size(); ++j) {
            AnimationInfo animInfo;
            animInfo.frameCount = model->getObject(index)->skeleton->joints[0]->animations[j]->frameCount;
            animInfo.name = model->getObject(index)->skeleton->joints[0]->animations[j]->name;
            model->getObject(index)->animationsInfo.push_back(animInfo);
        }
    }
}

void MgrImportFile::loadHitboxesNames(int index)
{
    if(importedHitboxes->importedHitboxesAvailableForObject(index))
    {
        HitboxPtr hitbox;
        for(int i=0;i<importedHitboxes->getObjectHitboxCount(index);++i)
        {
            hitbox = importedHitboxes->getHitbox(index,i);
            hitbox->name = model->getObject(index)->skeleton->joints[hitbox->jointIdx]->name;
        }
    }
}

void MgrImportFile::loadModel(std::fstream& file) {
    std::memset(block,0x00,blockSize*sizeof(char));
	int objIdx = -1;

    while(!errorFlag){
        file.read(block, blockSize * sizeof(char));

        objIdx = loadObjectData(file, objIdx);

        if(errorFlag==2) loadHitboxData(file, objIdx);
        if(errorFlag==2) checkFileEnd(file);
    }

    if(errorFlag==3) errorFlag = 0;
}

int MgrImportFile::loadObjectData(std::fstream& file, int objIdx)
{
    if (strcmp(block, FILEHDR::objectHeader) == 0) {
        std::unique_ptr<MgrImportObject> object = std::make_unique<MgrImportObject>(blockSize);
        if (object->loadObject(file) != 0) {
            errorFlag = 1;
            return objIdx;
        }
        model->loadObject(object->getObject());

        errorFlag = 0;
        return objIdx + 1;
    }

    errorFlag = 2;
    return objIdx;
}

void MgrImportFile::loadHitboxData(std::fstream& file, int objIdx)
{
    if (strcmp(block, FILEHDR::hitboxHeader) == 0) {
        if(importedHitboxes->loadHitbox(file,objIdx) != 0){
            errorFlag = 1;
            return;
        }
        errorFlag = 0;
        return;
    }
    errorFlag = 2;
}

void MgrImportFile::loadImportedHitboxesToBasicModel()
{
    for(int i=0;i<model->getObjectsCount();++i){
        HitboxPtr mainHitbox = importedHitboxes->getMainHitbox(i);
        model->loadMainHitbox(mainHitbox,i);

        for(int j=0;j<importedHitboxes->getObjectHitboxCount(i);++j){
            HitboxPtr hitbox = importedHitboxes->getHitbox(i,j);
            model->loadHitbox(hitbox,i,hitbox->jointIdx);
        }
    }
}

void MgrImportFile::checkFileEnd(std::fstream& file)
{
    if (strcmp(block, FILEHDR::modelFooter) == 0){
        errorFlag = 3;
        return;
    }
    else if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }
    errorFlag = 0;
}

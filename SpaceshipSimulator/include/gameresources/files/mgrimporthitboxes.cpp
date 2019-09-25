//#include "mgrimporthitboxes.h"
#include <stdafx.h>

int MgrImportHitboxes::loadHitbox(std::fstream& file, int objectIdx) {
    HitboxPtr hitbox = std::make_shared<Hitbox>();
    int bytesRead = 0;

    initHitboxStorageStructures(hitbox, objectIdx);

    bytesRead += loadHitboxJointIndex(hitbox,file);

    if(!errorFlag) bytesRead += loadHitboxDamageMultiplier(hitbox, file);

    if(!errorFlag) bytesRead += loadHitboxLocalAxis(hitbox,file);

    if(!errorFlag) bytesRead += loadHitboxBasicVertices(hitbox,file);

    if(!errorFlag) bytesRead += loadHitboxBasicNormals(hitbox,file);

    checkHitboxEndSection(file, bytesRead);

    return errorFlag;
}

HitboxPtr MgrImportHitboxes::getMainHitbox(int objectIdx)
{
    MainHitboxMap::iterator it = importedMainHitboxes.find(objectIdx);
    if (it != importedMainHitboxes.end())
        return it->second;
    else return nullptr;
}

HitboxPtr MgrImportHitboxes::getHitbox(int objectIdx, int hitboxIdx)
{
    HitboxMap::iterator it = importedHitboxes.find(objectIdx);
    if (it != importedHitboxes.end()) {
        if (hitboxIdx >= 0 && hitboxIdx < it->second.size())
            return it->second[hitboxIdx];
        else return nullptr;
    }
    else return nullptr;
}

bool MgrImportHitboxes::importedHitboxesAvailableForObject(int objectIdx)
{
    HitboxMap::iterator it = importedHitboxes.find(objectIdx);
    if (it != importedHitboxes.end())
        return true;
    else return false;
}

int MgrImportHitboxes::getObjectHitboxCount(int objectIdx)
{
    HitboxMap::iterator it = importedHitboxes.find(objectIdx);
    if (it != importedHitboxes.end())
        return it->second.size();
    else return 0;
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void MgrImportHitboxes::initHitboxStorageStructures(HitboxPtr hitbox,int objectIdx)
{
    MainHitboxMap::iterator it1 = importedMainHitboxes.find(objectIdx);
    HitboxMap::iterator it2 = importedHitboxes.find(objectIdx);
    if (it1 == importedMainHitboxes.end())
        importedMainHitboxes.insert(MainHitboxMap::value_type(objectIdx, hitbox));
    else {
        if (it2 == importedHitboxes.end()) {
            std::pair<HitboxMap::iterator, bool> p;
            p = importedHitboxes.insert(HitboxMap::value_type(objectIdx, std::vector<HitboxPtr>()));
            it2 = p.first;
        }

        it2->second.push_back(hitbox);
    }
}

int MgrImportHitboxes::loadHitboxJointIndex(HitboxPtr hitbox,std::fstream& file)
{
    file.read((char*)(&(hitbox->jointIdx)), sizeof(int));
    if ((file.rdstate()&std::fstream::eofbit) != 0)
        errorFlag = 1;
    else errorFlag =0;

    return sizeof(int);
}

int MgrImportHitboxes::loadHitboxDamageMultiplier(HitboxPtr hitbox, std::fstream& file)
{
    file.read((char*)(&(hitbox->damageMultiplier)), sizeof(double));
    if ((file.rdstate()&std::fstream::eofbit) != 0)
        errorFlag = 1;
    else errorFlag =0;

    return sizeof(double);
}

int MgrImportHitboxes::loadHitboxLocalAxis(HitboxPtr hitbox, std::fstream& file)
{
    AxisStruct axis;

    file.read((char*)(&axis), sizeof(AxisStruct));
    if ((file.rdstate()&std::fstream::eofbit) != 0)
        errorFlag = 1;
    else errorFlag =0;

    hitbox->localAxis.x = axis.GetX_axis();
    hitbox->localAxis.y = axis.GetY_axis();
    hitbox->localAxis.z = axis.GetZ_axis();

    return sizeof(AxisStruct);
}

int MgrImportHitboxes::loadHitboxBasicVertices(HitboxPtr hitbox, std::fstream& file)
{
    float basicVertices[8 * 3];

    file.read((char*)basicVertices, 24 * sizeof(float));
    if ((file.rdstate()&std::fstream::eofbit) != 0)
        errorFlag = 1;
    else errorFlag =0;

    for (int i = 0; i < 8; ++i) {
        hitbox->basicVertices[i].x = basicVertices[i * 3 + 0];
        hitbox->basicVertices[i].y = basicVertices[i * 3 + 1];
        hitbox->basicVertices[i].z = basicVertices[i * 3 + 2];
    }

    return (24 * sizeof(float));
}

int MgrImportHitboxes::loadHitboxBasicNormals(HitboxPtr hitbox, std::fstream& file)
{
    float basicNormals[6 * 3];

    file.read((char*)basicNormals, 18 * sizeof(float));
    if ((file.rdstate()&std::fstream::eofbit) != 0)
        errorFlag = 1;
    else errorFlag =0;

    for (int i = 0; i < 6; ++i) {
        hitbox->basicNormals[i].x = basicNormals[i * 3 + 0];
        hitbox->basicNormals[i].y = basicNormals[i * 3 + 1];
        hitbox->basicNormals[i].z = basicNormals[i * 3 + 2];
    }

    return (18 * sizeof(float));
}

void MgrImportHitboxes::checkHitboxEndSection(std::fstream& file, int bytesRead)
{
    std::memset(block,0x00,blockSize*sizeof(char));
    int blockComplement = blockSize - (bytesRead%blockSize);

    file.read(block, blockComplement * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }

    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }

    if (strncmp(block, FILEHDR::hitboxFooter, sizeof(FILEHDR::hitboxFooter) / sizeof(char)) != 0){
        errorFlag = 4;
        return;
    }

    errorFlag = 0;
}

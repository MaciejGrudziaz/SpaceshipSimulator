#ifndef MGRIMPORTHITBOXES_H
#define MGRIMPORTHITBOXES_H

#include "../gameobjects/hitbox.h"
#include "exportimportstruct.h"
#include "mgrimport.h"
#include <fstream>

class MgrImportHitboxes: public MgrImport
{
    typedef std::map<int, std::vector<HitboxPtr> > HitboxMap;
    HitboxMap importedHitboxes;
    typedef std::map<int, HitboxPtr> MainHitboxMap;
    MainHitboxMap importedMainHitboxes;

    void initHitboxStorageStructures(HitboxPtr hitbox,int objectIdx);
    int loadHitboxJointIndex(HitboxPtr hitbox,std::fstream& file);
    int loadHitboxDamageMultiplier(HitboxPtr hitbox, std::fstream& file);
    int loadHitboxLocalAxis(HitboxPtr hitbox, std::fstream& file);
    int loadHitboxBasicVertices(HitboxPtr hitbox, std::fstream& file);
    int loadHitboxBasicNormals(HitboxPtr hitbox, std::fstream& file);
    void checkHitboxEndSection(std::fstream& file, int bytesRead);

public:
    MgrImportHitboxes(int blockSize=0): MgrImport(blockSize) { }
    ~MgrImportHitboxes() { }

    int loadHitbox(std::fstream& file,int objectIdx);

    HitboxPtr getMainHitbox(int objectIdx);
    HitboxPtr getHitbox(int objectIdx, int hitboxIdx);
    bool importedHitboxesAvailable() { return (importedHitboxes.size()>0); }
    bool importedHitboxesAvailableForObject(int objectIdx);
    int getObjectHitboxCount(int objectIdx);
};

typedef std::shared_ptr<MgrImportHitboxes> MgrImportHitboxesPtr;

#endif // MGRIMPORTHITBOXES_H

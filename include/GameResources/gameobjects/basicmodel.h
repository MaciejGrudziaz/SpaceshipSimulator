#ifndef BASICMODEL_H
#define BASICMODEL_H

#include <vector>
#include "basicobject.h"
#include "hitbox.h"

struct BasicObjectImport{
    BasicObjectImport(BasicObjectPtr object) {
        this->object = object;
        hitboxes = std::make_shared<HitboxMap>();
    }

    BasicObjectPtr object;
    HitboxPtr mainHitbox;
    HitboxMapPtr hitboxes;
};

typedef std::shared_ptr<BasicObjectImport> BasicObjectImportPtr;

class BasicModel{
    std::vector<BasicObjectImportPtr> objects;

public:
	BasicModel() { }

    void loadObject(BasicObjectPtr object);
    void loadMainHitbox(HitboxPtr hitbox, int objectIdx);
    void loadHitbox(HitboxPtr hitbox, int objectIdx, int jointIdx);
    void loadHitboxes(HitboxMapPtr hitboxMap, int objectIdx);

    int getObjectsCount()const { return objects.size(); }
    BasicObjectPtr getObject(int objectIdx)const;
    HitboxPtr getMainHitbox(int objectIdx)const;
    HitboxMapPtr getHitboxes(int objectIdx)const;

    ~BasicModel() {}
};

typedef std::shared_ptr<BasicModel> BasicModelPtr;

#endif //BASICMODEL_H

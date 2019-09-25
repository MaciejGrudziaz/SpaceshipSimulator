#include <stdafx.h>

void BasicModel::loadObject(BasicObjectPtr object)
{
    //if (object != nullptr) objects.push_back(object);
    if(object){
        objects.push_back(std::make_shared<BasicObjectImport>(object));
    }
}

void BasicModel::loadMainHitbox(HitboxPtr hitbox, int objectIdx)
{
    if(objectIdx >= 0 && objectIdx < objects.size())
        objects[objectIdx]->mainHitbox = hitbox;
}

void BasicModel::loadHitbox(HitboxPtr hitbox, int objectIdx, int jointIdx)
{
    if(objectIdx >= 0 && objectIdx < objects.size())
        objects[objectIdx]->hitboxes->insert(HitboxMap::value_type(jointIdx,hitbox));
}

void BasicModel::loadHitboxes(HitboxMapPtr hitboxMap, int objectIdx)
{
    if(objectIdx >= 0 && objectIdx < objects.size())
        objects[objectIdx]->hitboxes = hitboxMap;
}

BasicObjectPtr BasicModel::getObject(int objectIdx)const
{
    if(objectIdx >= 0 && objectIdx < objects.size())
        return objects[objectIdx]->object;
}

HitboxPtr BasicModel::getMainHitbox(int objectIdx)const
{
    if(objectIdx >= 0 && objectIdx < objects.size())
        return objects[objectIdx]->mainHitbox;
}

HitboxMapPtr BasicModel::getHitboxes(int objectIdx)const
{
    if(objectIdx >= 0 && objectIdx < objects.size())
        return objects[objectIdx]->hitboxes;
}

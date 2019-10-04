#pragma once

#include <GameResources/gameobjects/gameobject.h>
#include "../gamelogic/collisiondetectionalgorithm.h"
#include "../gamelogic/collisions.h"
#include <thread>
#include <mutex>

class CollisionsManager
{
public:
	CollisionsManager();

	void process();
	void invalidate();

	void run();

	void registerCollisionObject(GameObjectPtr object);

private:
	std::vector<std::shared_ptr<Property<GameObject> > > collisionObjects;

	std::thread collisionDetectionThread;
	std::mutex updateMutex;
	bool updateFlag;
	bool runFlag;

	void processCollisions();
	void checkAllCollisions();
};

typedef std::shared_ptr<CollisionsManager> CollisionsManagerPtr;

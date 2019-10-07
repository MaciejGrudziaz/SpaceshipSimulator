#pragma once

#include <GameResources/gameobjects/gameobject.h>
#include "../gamelogic/collisiondetectionalgorithm.h"
#include "../gamelogic/collisions.h"
#include "../gamelogic/laserbeamhitdetection.h"
#include <thread>
#include <mutex>

class CollisionsManager
{
public:
	CollisionsManager();

	void invalidate();

	void process();

	void registerCollisionObject(GameObjectPtr object);
	void registerHitDetectionObject(GameObjectPtr object);

	void pause();
	void run();

	bool isThreadPaused();

private:
	int warmupCounter;
	const int warmupLimitCount = 10;
	std::vector<std::shared_ptr<Property<GameObject> > > collisionObjects;
	std::vector<std::shared_ptr<Property<GameObject> > > hitDetectionObjects;

	std::thread collisionDetectionThread;
	bool runFlag;
	bool pauseFlag;
	bool isPaused;

	void processCollisions();

	void checkHitboxCollisions();
	void checkHitDetection();
};

typedef std::shared_ptr<CollisionsManager> CollisionsManagerPtr;

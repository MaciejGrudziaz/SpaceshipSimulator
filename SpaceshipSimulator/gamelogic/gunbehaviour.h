#pragma once

#include "../gameengine.h"
#include "../gameobjects/gameresources.h"
#include "../gameobjects/laserbeam.h"

struct DelayControl
{
	Time delay;
	bool useDelay;
	bool setTimer;
	bool delayEnded;

	DelayControl()
		: useDelay(false)
		, setTimer(false)
		, delayEnded(false)
	{}

	void reset();

	bool set(float fireRate);
};

class GunShoot : public Property<GameObject>
{
public:
	GunShoot(const std::string& name, GameObject& object)
		: Property<GameObject>(name, object)
	{}

	void init()override;
	void process()override;
	void invalidate()override;

	void setDelay();

private:
	enum direction { stop, forward, backward };
	float fireRate;		// 1/s
	float maxMoveDist;
	float currMoveDist;
	float barrelSpeed;
	direction dir;

	DelayControl delay;

	void shootAnimation();
	void shoot();
};

void registerSpaceshipGunBehaviours(GameEngine& engine);

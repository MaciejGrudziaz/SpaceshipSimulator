#include "gunbehaviour.h"

void DelayControl::reset()
{
	setTimer = false;
	delayEnded = false;
}

bool DelayControl::set(float fireRate)
{
	if (!setTimer) {
		setTimer = true;
		delay.setTimer(std::make_shared<TimerOptions<Hz> >(fireRate * 2.0f), [this](int a) {
			this->delayEnded = true;
		});
	}
	return delayEnded;
}

void GunShoot::init()
{
	fireRate = 10.0f;
	maxMoveDist = 0.5f;
	currMoveDist = 0.0f;
	dir = stop;
	barrelSpeed = (2.0f * maxMoveDist) / (1.0f / fireRate);
}

void GunShoot::process()
{
	if (GameEngine::getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
	{
		if (dir == stop)
		{
			if (delay.useDelay)
			{
				if(delay.set(fireRate))
					dir = backward;
			}
			else dir = backward;
		}

		shoot();
	}
	else {
		if (dir != stop)
		{
			delay.reset();
			shoot();
		}
	}

	glm::vec3 pos = object.getTransform().getPosition();
	pos.y = -currMoveDist;
	object.getTransform().setPosition(pos);
}

void GunShoot::shoot()
{
	if (dir == backward)
	{
		currMoveDist += barrelSpeed * Time::deltaTime;

		if (currMoveDist > maxMoveDist)
		{
			currMoveDist = maxMoveDist;
			dir = forward;
		}
	}
	else if (dir == forward)
	{
		currMoveDist -= barrelSpeed * Time::deltaTime;

		if (currMoveDist < 0.0f)
		{
			currMoveDist = 0.0f;
			dir = stop;
		}
	}
}

void GunShoot::invalidate()
{}

void GunShoot::setDelay()
{
	delay.useDelay = true;
}

void registerSpaceshipGunBehaviours(GameEngine& engine)
{
	GameObjectPtr gun = engine.getResources()->spaceship->getChild("gun_left");
	if (gun->isUsable())
	{
		gun->addProperty<GunShoot>("shoot");
	}

	gun = engine.getResources()->spaceship->getChild("gun_right");
	if (gun->isUsable())
	{
		gun->addProperty<GunShoot>("shoot");
		std::shared_ptr<Property<GameObject> > prop = gun->getProperty("shoot");
		if (prop->isUsable())
		{
			static_cast<GunShoot&>(*prop).setDelay();
		}
	}
}

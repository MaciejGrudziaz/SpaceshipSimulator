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
	fireRate = 5.0f;
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
				if (delay.set(fireRate))
				{
					dir = backward;
					shoot();
				}
			}
			else
			{
				dir = backward;
				shoot();
			}
		}

		shootAnimation();
	}
	else {
		if (dir != stop)
		{
			delay.reset();
			shootAnimation();
		}
	}

	glm::vec3 pos = object.getTransform().getPosition();
	pos.y = -currMoveDist;
	object.getTransform().setPosition(pos);
}

void GunShoot::shoot()
{
	Spaceship& parent = static_cast<Spaceship&>(object.getParent());

	LaserBeamPtr beam = std::make_shared<LaserBeam>();
	beam->loadViewMatrixPtr(parent.getCamera()->getViewPtr());
	beam->loadProjectionMatrixPtr(parent.getProjectionMat());
	beam->setSpeed(20.0f);

	glm::vec3 pos;
	if(delay.useDelay) pos = parent.getTransform().getTransformMat() * glm::vec4(0.3f, 2.6f, -0.3f, 1.0f);
	else pos = parent.getTransform().getTransformMat() * glm::vec4(-0.3f, 2.6f, -0.3f, 1.0f);

	beam->launch(pos, parent.getTransform().getRotationQuat());
	
	parent.addLaserBeam(beam);
}

void GunShoot::shootAnimation()
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

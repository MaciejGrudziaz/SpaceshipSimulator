#include "asteroid.h"

Asteroid::Asteroid()
{}

void Asteroid::init()
{
	StandardGameObject::init();

	initParticleSystems();

	explosionParticles->init();
	explosionFragmentsParticles->init();
}

void Asteroid::initParticleSystems()
{
	explosionParticles = std::make_shared<ParticleSystem>();
	explosionParticles->setParticlesCount(4000);
	explosionParticles->setParticlesMaxSpeed(6.5f);
	explosionParticles->setParticlesMaxLifetime(1.0f);
	explosionParticles->setParticlesSize(0.5f);
	explosionParticles->setEvenSpread();
	explosionParticles->setColors(glm::vec3(1.0f, 0.55f, 0.1f), glm::vec3(0.7f, 0.7f, 0.7f));
	explosionParticles->registerCamera(camera);
	explosionParticles->getTransform().setPosition(glm::vec3(0.0f));
	explosionParticles->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE);

	ParticleSystemData data;
	data.particleTexture = "sprites/asteroid_particle.png";
	data.vertexShaderFilename = "shaders/particle.vert";
	data.fragmentShaderFilename = "shaders/particle.frag";

	ModelExternalUniforms uniforms;
	uniforms.view = camera->getViewPtr();
	uniforms.projection = projection;

	explosionParticles->loadRenderer(data, uniforms);

	explosionFragmentsParticles = std::make_shared<ParticleSystem>();
	explosionFragmentsParticles->setParticlesCount(2000);
	explosionFragmentsParticles->setParticlesMaxSpeed(5.0f);
	explosionFragmentsParticles->setParticlesMaxLifetime(3.0f);
	explosionFragmentsParticles->setParticlesSize(0.6f);
	explosionFragmentsParticles->setEvenSpread();
	explosionFragmentsParticles->setColors(glm::vec3(1.0f, 1.0f, 1.0f), glm::vec3(0.7f, 0.7f, 0.7f));
	explosionFragmentsParticles->registerCamera(camera);
	explosionFragmentsParticles->getTransform().setPosition(glm::vec3(0.0f));
	explosionFragmentsParticles->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE);

	explosionFragmentsParticles->loadRenderer(data, uniforms);
}

void Asteroid::process()
{
	StandardGameObject::process();

	if (isActive())
	{
		glm::vec3 pos = transform.getPosition();
		glm::vec3 rot = transform.getRotation();

		glm::vec3 worldSpeedVec(0.0f, -(*worldSpeed), 0.0f);
		pos += (linearSpeed + worldSpeedVec) * Time::deltaTime;
		rot += rotSpeed * Time::deltaTime;

		for (int i = 0; i < 3; ++i)
		{
			if (rot[i] > 360.0f) rot[i] = rot[i] - 360.0f;
			if (rot[i] < -360.0f) rot[i] = rot[i] + 360.0f;
		}

		transform.setPosition(pos);
		transform.setRotation(rot);
	}

	explosionParticles->setParentTransform(transform.getTransformMat());
	explosionFragmentsParticles->setParentTransform(transform.getTransformMat());
	explosionParticles->process();
	explosionFragmentsParticles->process();
}

void Asteroid::invalidate()
{
	StandardGameObject::invalidate();

	explosionParticles->invalidate();
	explosionFragmentsParticles->invalidate();
}

void Asteroid::deepCopy(const Asteroid& asteroid)
{
	StandardGameObject::deepCopy(asteroid);

	this->worldSpeed = asteroid.worldSpeed;
	this->rotSpeed = asteroid.rotSpeed;
	this->linearSpeed = asteroid.linearSpeed;
	this->camera = asteroid.camera;
	this->projection = asteroid.projection;
}

void Asteroid::setActive(bool val)
{
	StandardGameObject::setActive(val);


}

void Asteroid::registerWorldSpeed(std::shared_ptr<float> speed)
{
	worldSpeed = speed;
}

void Asteroid::registerCamera(CameraPtr camera)
{
	this->camera = camera;
}

void Asteroid::registerProjectionMatrixPtr(ConstMat4Ptr projection)
{
	this->projection = projection;
}

void Asteroid::setRotSpeed(glm::vec3 rotSpeed)
{
	this->rotSpeed = rotSpeed;
}

void Asteroid::setLinearSpeed(glm::vec3 linSpeed)
{
	this->linearSpeed = linSpeed;
}

void Asteroid::dealDamage()
{
	if (!explosionFragmentsParticles->isRunning())
	{
		setActive(false);

		explosionParticles->setSingleSpread();
		explosionParticles->launch();

		explosionFragmentsParticles->setSingleSpread();
		explosionFragmentsParticles->launch();
	}
	setActive(false);
}

RenderObjectPtr Asteroid::getExplosionParticlesRenderer()const
{
	return explosionParticles->getRenderer();
}

RenderObjectPtr Asteroid::getExplosionFragmentsParticlesRenderer()const
{
	return explosionFragmentsParticles->getRenderer();
}

//ParticleSystemPtr Asteroid::getExplosionParticlesSystem()const
//{
//	return explosionParticles;
//}
//
//ParticleSystemPtr Asteroid::getExplosionFragmentsParticlesSystem()const
//{
//	return explosionFragmentsParticles;
//}

bool Asteroid::isParticleSystemRunning()const
{
	return explosionFragmentsParticles->isRunning();
}

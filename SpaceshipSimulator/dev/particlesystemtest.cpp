#include "particlesystemtest.h"

void initializeParticleSystem(GameEngine& engine)
{
	ParticleSystemPtr particles = std::make_shared<ParticleSystem>();
	particles->setParticlesCount(5000);
	particles->setParticlesMaxSpeed(10.0f);
	particles->setParticlesMaxLifetime(0.3f);
	particles->setParticlesSize(0.5f);
	particles->setParticlesSpreadCone(-15.0f, 15.0f);
	particles->setColors(glm::vec3(1.0f, 0.92f, 0.01f), glm::vec3(0.2f, 0.2f, 0.2f));
	particles->setBlendingFunctions(GL_SRC_ALPHA, GL_ONE);

	particles->registerCamera(engine.getResources()->camera);

	ParticleSystemData data;
	data.particleTexture = "sprites/smoke.png";
	data.vertexShaderFilename = "shaders/particle.vert";
	data.fragmentShaderFilename = "shaders/particle.frag";

	ModelExternalUniforms uniforms;
	uniforms.view = engine.getResources()->camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	particles->loadRenderer(data, uniforms);

	//particles->addProperty<PlayerInput>("input");
	particles->getTransform().setRotation(glm::vec3(90.0f, 0.0f, 0.0f));

	engine.getResources()->particles = particles;
	engine.addRenderer(particles->getRenderer());

	particles->init();

	ParticleSystemPtr particles2 = std::make_shared<ParticleSystem>();
	particles2->setParticlesCount(5000);
	particles2->setParticlesMaxSpeed(2.0f);
	particles2->setParticlesMaxLifetime(0.6f);
	particles2->setParticlesSize(0.5f);
	particles2->setParticlesSpreadCone(-90.0f, 90.0f);
	particles2->setColors(glm::vec3(1.0f, 0.92f, 0.01f), glm::vec3(1.0f, 1.0f, 1.0f));
	particles2->registerCamera(engine.getResources()->camera);
	particles2->getTransform().setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
	particles2->loadRenderer(data, uniforms);

	engine.getResources()->particles2 = particles2;
	engine.addRenderer(particles2->getRenderer());

	particles2->init();
}

void processParticleSystem(GameEngine& engine)
{
	engine.getResources()->particles->process();
	engine.getResources()->particles2->process();
}

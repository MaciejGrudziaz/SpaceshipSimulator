#include "particlesystemtest.h"

void initializeParticleSystem(GameEngine& engine)
{
	ParticleSystemPtr particles = std::make_shared<ParticleSystem>();
	particles->setParticlesCount(2000);
	particles->setParticlesOrigin(glm::vec3(0.0f,-10.0f,0.0f));
	particles->setParticlesDirection(glm::vec3(0.0f, 0.0f, 0.0f));
	particles->setParticlesMaxSpeed(5.0f);
	particles->setParticlesMaxLifetime(2.0f);
	particles->setParticlesSize(0.7f);
	particles->setParticlesSpreadCone(-45.0f, 45.0f);

	ParticleSystemData data;
	data.particleTexture = "sprites/smoke.png";
	data.vertexShaderFilename = "shaders/particle.vert";
	data.fragmentShaderFilename = "shaders/particle.frag";

	ModelExternalUniforms uniforms;
	uniforms.view = engine.getResources()->camera->getViewPtr();
	uniforms.projection = engine.getProjectionMatPtr();

	particles->load(data, uniforms);

	engine.getResources()->particles = particles;
	engine.addRenderer(particles->getRenderer());
}

void processParticleSystem(GameEngine& engine)
{
	engine.getResources()->particles->update(engine.getResources()->camera->getTransform().getPosition());
}

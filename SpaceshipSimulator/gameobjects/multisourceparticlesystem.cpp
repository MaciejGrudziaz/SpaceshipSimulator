#include "multisourceparticlesystem.h"

MultiSourceParticleSystem::MultiSourceParticleSystem()
	: renderer(std::make_shared<MultiSourceParticleRenderer>())
{}

void MultiSourceParticleSystem::init()
{
	GameObject::init();

	renderer->init();

	//for (auto particleSys : particleSystems)
	//{
	//	particleSys->particles->init();
	//}
}

void MultiSourceParticleSystem::process()
{
	GameObject::process();

	for (auto particleSys : particleSystems)
	{
		particleSys->particles->process();
		particleSys->renderParams->isActive = particleSys->particles->isActive();
	}
}

void MultiSourceParticleSystem::invalidate()
{
	GameObject::invalidate();

	renderer->invalidate();

	//for (auto particleSys : particleSystems)
	//{
	//	particleSys->particles->invalidate();
	//}
}

void MultiSourceParticleSystem::setActive(bool val)
{
	GameObject::setActive(val);
}

void MultiSourceParticleSystem::registerCamera(CameraPtr camera)
{
	this->camera = camera;
}

void MultiSourceParticleSystem::registerParticleSystem(ParticleSystemPtr particleSystem)
{
	ParticleSysRenderParamsPtr renderParams = std::make_shared<ParticleSysRenderParams>(
		particleSystem->getParticlesPosSizeBuffer(), 
		particleSystem->getParticlesColorBuffer() );

	renderParams->blendFunctions = particleSystem->getBlendFunctions();
	renderParams->isActive = particleSystem->isActive();
	renderParams->particleCount = particleSystem->getParticlesCount();

	ParticleSystemParamsPtr particleSysParams = std::make_shared<ParticleSystemParams>();
	particleSysParams->particles = particleSystem;
	particleSysParams->renderParams = renderParams;

	particleSystems.push_back(particleSysParams);
}

void MultiSourceParticleSystem::loadRenderer(const ParticleSystemData& data, const ModelExternalUniforms& uniforms)
{
	renderer->loadShader(data.vertexShaderFilename, data.fragmentShaderFilename);
	initUniforms(uniforms);
	renderer->loadTexture(data.particleTexture);
}

void MultiSourceParticleSystem::initUniforms(const ModelExternalUniforms& uniforms)
{
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");
	UniformDataVec3Ptr cameraUpUniform = std::make_shared<UniformDataVec3>("cameraUp");
	UniformDataVec3Ptr cameraRightUniform = std::make_shared<UniformDataVec3>("cameraRight");

	viewUniform->mat = uniforms.view;
	projectionUniform->mat = uniforms.projection;
	cameraUpUniform->vec = camera->getUpVecPtr();
	cameraRightUniform->vec = camera->getRightVecPtr();

	renderer->addUniform(viewUniform);
	renderer->addUniform(projectionUniform);
	renderer->addUniform(cameraUpUniform);
	renderer->addUniform(cameraRightUniform);
}

MultiSourceParticleRendererPtr MultiSourceParticleSystem::getRenderer()const
{
	return renderer;
}

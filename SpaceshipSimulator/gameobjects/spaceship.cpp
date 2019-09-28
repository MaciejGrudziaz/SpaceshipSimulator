#include "spaceship.h"

Spaceship::Spaceship()
	: renderer(std::make_shared<RenderObject>())
	, mainHitbox(std::make_shared<Hitbox>())
	, modelTransform(std::make_shared<glm::mat4>(1.0f))
{}

void Spaceship::load(const SpaceshipData& data, const SpaceshipUniforms& uniforms)
{
	std::unique_ptr<SpaceshipLoadData> loadSpaceship;
	std::fstream file(data.textureFilename);
	if (file.is_open())
	{
		file.close();
		renderer = std::make_shared<TextureRenderObject>();
		loadSpaceship = std::make_unique<TextureSpaceshipLoadData>(*this, data, uniforms);
	}
	else {
		renderer = std::make_shared<RenderObject>();
		loadSpaceship = std::make_unique<CleanSpaceshipLoadData>(*this, data, uniforms);
	}

	loadSpaceship->load();
}

void Spaceship::process() 
{
	GameObject::process();

	*modelTransform = getTransform().getTransformMat();
}

std::shared_ptr<RenderObject> Spaceship::getRenderer()const
{
	return renderer;
}

Spaceship::error Spaceship::getErrorCode()
{
	error code;
	if (!errorCode.empty())
	{
		code = errorCode.top();
		errorCode.pop();
	}
	else code = NO_ERROR;

	return code;
}

#include "spaceship.h"

Spaceship::Spaceship()
{
	beamsColor = std::make_shared<glm::vec3>(1.0f, 0.0f, 0.0f);
	beamsRenderer = std::make_shared<BeamsRenderer>(beamsBuffer);
}

void Spaceship::init()
{
	StandardGameObject::init();

	beamsRenderer->loadShaders();
	beamsRenderer->init();
	loadUniforms();
	loadAttribPointers();
}

void Spaceship::loadUniforms()
{
	UniformDataVec3Ptr colorUniform = std::make_shared<UniformDataVec3>("color");
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");

	colorUniform->vec = beamsColor;
	viewUniform->mat = camera->getViewPtr();
	projectionUniform->mat = projectionMat;

	beamsRenderer->addUniform(colorUniform);
	beamsRenderer->addUniform(viewUniform);
	beamsRenderer->addUniform(projectionUniform);	
}

void Spaceship::loadAttribPointers()
{
	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	beamsRenderer->addShaderAttribute(posAttrib);
}

void Spaceship::process()
{
	StandardGameObject::process();

	std::for_each(laserShots.begin(), laserShots.end(), [](auto shot) {
		shot->process();
	});

	updateBeamsBuffer();
}

void Spaceship::updateBeamsBuffer()
{
	glm::vec3 pos;
	if (beamsBuffer.size() / 6 < laserShots.size())
	{
		for (int i = beamsBuffer.size() / 6; i < laserShots.size(); ++i)
		{
			pos = laserShots[i]->getTransform().getPosition();

			for (int j = 0; j < 3; ++j)
				beamsBuffer.push_back(pos[j]);

			pos = pos - laserShots[i]->getDirection() * laserShots[i]->getLength();

			for (int j = 0; j < 3; ++j)
				beamsBuffer.push_back(pos[j]);
		}
	}

	for (int i = 0; i < laserShots.size(); ++i)
	{
		pos = laserShots[i]->getTransform().getPosition();

		for (int j = 0; j < 3; ++j)
			beamsBuffer[6 * i + j] = pos[j];

		pos = pos - laserShots[i]->getDirection() * laserShots[i]->getLength();

		for (int j = 0; j < 3; ++j)
			beamsBuffer[6 * i + j + 3] = pos[j];
	}

	beamsRenderer->setUpdateBufferFlag();
}


void Spaceship::loadGuns()
{
	SpaceshipGunPtr gunLeft = std::make_shared<SpaceshipGun>();
	gunLeft->setName("gun_left");

	SpaceshipData data;
	data.jointIdx = 1;
	data.model = getModel("spaceship");
	data.renderer = renderer;
	data.textureAvailable = textureAvailable;

	gunLeft->load(data);

	SpaceshipGunPtr gunRight = std::make_shared<SpaceshipGun>();
	gunRight->setName("gun_right");

	data.jointIdx = 3;

	gunRight->load(data);

	addChild(gunLeft);
	addChild(gunRight);
}

void Spaceship::addLaserBeam(LaserBeamPtr beam)
{
	laserShots.push_back(beam);
}

void Spaceship::registerCamera(CameraPtr camera)
{
	this->camera = camera;
}

void Spaceship::registerProjectionMat(ConstMat4Ptr projection)
{
	projectionMat = projection;
}

CameraPtr Spaceship::getCamera()const
{
	return camera;
}

ConstMat4Ptr Spaceship::getProjectionMat()const
{
	return projectionMat;
}

BeamsRendererPtr Spaceship::getBeamsRenderer()const
{
	return beamsRenderer;
}

void Spaceship::loadStandardBufferData()
{
	std::vector<float> buffer;

	BasicObjectPtr model = getModel("spaceship");

	if (model->isUsable())
	{
		std::for_each(model->faces.begin(), model->faces.end(), [&model, &buffer](auto face) {
			bool isBody = true;
			for (int i = 0; i < 3; ++i)
			{
				isBody = true;
				for (const BlendingIndexWeightPair& blendInfo : model->controlPoints[face.indices[i]].blendingInfo)
				{
					if (blendInfo.jointIndex != 0 && blendInfo.weight > 0.5f)
					{
						isBody = false;
						break;
					}
				}
				if (isBody)
				{
					for (int j = 0; j < 3; ++j)
					{
						buffer.push_back(model->controlPoints[face.indices[i]].coords[j]);
					}
				}
			}
		});
	}

	renderer->loadBuffer(buffer);
}

void Spaceship::loadTextureBufferData()
{
	std::vector<float> buffer;

	BasicObjectPtr model = getModel("spaceship");

	if (model->isUsable())
	{
		std::for_each(model->faces.begin(), model->faces.end(), [&model, &buffer](auto face) {
			bool isBody = true;
			for (int i = 0; i < 3; ++i)
			{
				isBody = true;
				for (const BlendingIndexWeightPair& blendInfo : model->controlPoints[face.indices[i]].blendingInfo)
				{
					if (blendInfo.jointIndex != 0 && blendInfo.weight > 0.5f)
					{
						isBody = false;
						break;
					}
				}
				if (isBody)
				{
					for (int j = 0; j < 3; ++j)
						buffer.push_back(model->controlPoints[face.indices[i]].coords[j]);
					buffer.push_back(face.uv[i].x);
					buffer.push_back(face.uv[i].y);
				}
			}
		});
	}

	renderer->loadBuffer(buffer);
}

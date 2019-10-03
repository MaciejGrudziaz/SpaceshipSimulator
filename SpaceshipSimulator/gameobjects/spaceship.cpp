#include "spaceship.h"

Spaceship::Spaceship()
	: inputRotation(0.0f)
{
	beamsColor = std::make_shared<glm::vec4>(0.0f, 1.0f, 0.0f, 0.8f);
	beamsSize = std::make_shared<glm::vec2>(0.2f, 3.0f);
	beamsRenderer = std::make_shared<TextureBeamsRenderer>(beamsBuffer);
}

void Spaceship::init()
{
	StandardGameObject::init();

	beamsRenderer->loadShader("shaders/texLaserShaders.vert", "shaders/texLaserShaders.frag");
	loadBeamsUniforms();
	beamsRenderer->init();

	beamsRenderer->loadBuffers();
	beamsRenderer->loadTexture("sprites/laser_beam_2.png");
}

void Spaceship::loadBeamsUniforms()
{
	UniformDataVec4Ptr colorUniform = std::make_shared<UniformDataVec4>("color");
	UniformDataVec2Ptr sizeUniform = std::make_shared<UniformDataVec2>("size");

	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");

	UniformDataVec3Ptr cameraUpUniform = std::make_shared<UniformDataVec3>("cameraUp");
	UniformDataVec3Ptr cameraRightUniform = std::make_shared<UniformDataVec3>("cameraRight");

	colorUniform->vec = beamsColor;
	sizeUniform->vec = beamsSize;

	viewUniform->mat = camera->getViewPtr();
	projectionUniform->mat = projectionMat;

	cameraUpUniform->vec = camera->getUpVecPtr();
	cameraRightUniform->vec = camera->getRightVecPtr();

	beamsRenderer->addUniform(colorUniform);
	beamsRenderer->addUniform(sizeUniform);
	beamsRenderer->addUniform(viewUniform);
	beamsRenderer->addUniform(projectionUniform);	
	beamsRenderer->addUniform(cameraUpUniform);
	beamsRenderer->addUniform(cameraRightUniform);
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
	if (beamsBuffer.size() / 7 < laserShots.size())
	{
		for (int i = beamsBuffer.size() / 7; i < laserShots.size(); ++i)
		{
			pos = laserShots[i]->getTransform().getPosition();

			for (int j = 0; j < 3; ++j)
				beamsBuffer.push_back(pos[j]);

			glm::quat q(laserShots[i]->getTransform().getRotationQuat());

			for (int j = 0; j < 4; ++j)
				beamsBuffer.push_back(q[j]);
		}
	}

	for (int i = 0; i < laserShots.size(); ++i)
	{
		pos = laserShots[i]->getTransform().getPosition();

		for (int j = 0; j < 3; ++j)
			beamsBuffer[7 * i + j] = pos[j];

		glm::quat q(laserShots[i]->getTransform().getRotationQuat());

		for (int j = 0; j < 4; ++j)
			beamsBuffer[7 * i + j + 3] = q[j];
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

TextureBeamsRendererPtr Spaceship::getBeamsRenderer()const
{
	return beamsRenderer;
}

void Spaceship::setInputMoveVec(const glm::vec3& val)
{
	inputMoveVec = val;
}

glm::vec3 Spaceship::getInputMoveVec()const
{
	return inputMoveVec;
}

void Spaceship::setInputRotation(float val)
{
	inputRotation = val;
}

float Spaceship::getInputRotation()const
{
	return inputRotation;
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

#include "spaceship.h"

Spaceship::Spaceship()
	: inputRotation(0.0f)
	, mouseInput(false)
{
	beamsColor = std::make_shared<glm::vec4>(0.0f, 1.0f, 0.0f, 0.8f);
	beamsSize = std::make_shared<glm::vec2>(0.2f, 3.0f);
	beamsRenderer = std::make_shared<TextureBeamsRenderer>(beamsBuffer);
	setName("spaceship");
	(*hitColor) = glm::vec3(0.4f, 0.4f, 1.0f);

	//maxShieldValue = 10;
	//currShieldValue = maxShieldValue;
	//maxLifeValue = 10;
	//currLifeValue = maxLifeValue;
	pointsScore = 0;
	guiShieldStatuBarFrac = std::make_shared<float>(1.0f);
	guiLifeStatusBarFrac = std::make_shared<float>(1.0f);
	guiPointsScore = std::make_shared<int>(0);

	initClearLaserShotsBuffer();
}

void Spaceship::initClearLaserShotsBuffer()
{
	laserShots.clear();
	laserShots = std::vector<LaserBeamPtr>(1000);
	for (int i = 0; i < laserShots.size(); ++i)
	{
		laserShots[i] = std::make_shared<LaserBeam>();
		laserShots[i]->setActive(false);
	}
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
	
	std::shared_ptr<Property<GameObject> > prop = getProperty("hit_detection");

	for (auto shot : laserShots)
	{
		if (shot->isActive())
		{
			shot->process();

			glm::vec4 clipPos = (*projectionMat) * camera->getView() * glm::vec4(shot->getTransform().getPosition(),1.0f);
			clipPos /= clipPos.w;
			for (int i = 0; i < 3; ++i)
			{
				if (clipPos[i] < -1.0f || clipPos[i] > 1.0f)
					shot->setActive(false);
			}
		}
	}

	updateBeamsBuffer();
}

void Spaceship::updateBeamsBuffer()
{
	//glm::vec3 pos;
	//if (beamsBuffer.size() / TextureBeamsRenderer::vertexAttribCount < laserShots.size())
	//{
	//	for (int i = beamsBuffer.size() / TextureBeamsRenderer::vertexAttribCount; i < laserShots.size(); ++i)
	//	{
	//		pos = laserShots[i]->getTransform().getPosition();

	//		for (int j = 0; j < 3; ++j)
	//			beamsBuffer.push_back(pos[j]);

	//		glm::quat q(laserShots[i]->getTransform().getRotationQuat());

	//		for (int j = 0; j < 4; ++j)
	//			beamsBuffer.push_back(q[j]);
	//	}
	//}

	//for (int i = 0; i < laserShots.size(); ++i)
	//{
	//	pos = laserShots[i]->getTransform().getPosition();

	//	for (int j = 0; j < 3; ++j)
	//		beamsBuffer[7 * i + j] = pos[j];

	//	glm::quat q(laserShots[i]->getTransform().getRotationQuat());

	//	for (int j = 0; j < 4; ++j)
	//		beamsBuffer[7 * i + j + 3] = q[j];
	//}

	//beamsBuffer.erase(beamsBuffer.begin() + (laserShots.size() * TextureBeamsRenderer::vertexAttribCount), beamsBuffer.end());

	//std::shared_ptr<Property<GameObject> > prop = getProperty("hit_detection");
	//if (prop->isUsable())
	//{
	//	LaserBeamHitDetection& hitDetectObj = static_cast<LaserBeamHitDetection&>(*prop);
	//	hitDetectObj.getLaserBeamsVec();
	//}

	int beamsBufferCurrIdx = 0;
	glm::vec3 pos;
	glm::quat rot;
	for(auto shot : laserShots)
	{
		if (shot->isActive())
		{
			pos = shot->getTransform().getPosition();
			rot = shot->getTransform().getRotationQuat();

			if (beamsBufferCurrIdx < beamsBuffer.size())
			{
				for (int i = 0; i < 3; ++i)
					beamsBuffer[beamsBufferCurrIdx + i] = pos[i];

				for (int i = 0; i < 4; ++i)
					beamsBuffer[beamsBufferCurrIdx + i + 3] = rot[i];

				beamsBufferCurrIdx += 7;
			}
			else {
				for (int i = 0; i < 3; ++i)
					beamsBuffer.push_back(pos[i]);

				for (int i = 0; i < 4; ++i)
					beamsBuffer.push_back(rot[i]);

				beamsBufferCurrIdx += 7;
			}
		}
	}

	//if (prop->isUsable())
	//{
	//	LaserBeamHitDetection& hitDetectObj = static_cast<LaserBeamHitDetection&>(*prop);
	//	hitDetectObj.releaseLaserBeamVec();
	//}

	if (beamsBufferCurrIdx < beamsBuffer.size())
	{
		beamsBuffer.erase(beamsBuffer.begin() + beamsBufferCurrIdx, beamsBuffer.end());
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
	int idx = findNotActiveLaserShot();
	if(idx!=-1) laserShots[idx] = beam;

	beam->setSpeed(laserBeamSpeed);

	auto colPtProperty = getProperty("hit_detection");
	if (colPtProperty->isUsable())
	{
		LaserBeamHitDetection& property = static_cast<LaserBeamHitDetection&>(*colPtProperty);
		property.addLaserBeam(beam);
	}
}

int Spaceship::findNotActiveLaserShot()
{
	static int idx = 0;
	bool found = false;

	for (int i = idx; i < laserShots.size(); ++i)
	{
		if (!laserShots[i]->isActive())
		{
			idx = i;
			found = true;
			break;
		}
	}

	if (!found)
	{
		for (int i = 0; i < idx; ++i)
		{
			if (!laserShots[i]->isActive())
			{
				idx = i;
				found = true;
				break;
			}
		}
	}

	if (!found) return -1;

	return idx;
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

//void Spaceship::registerWorldSpeed(std::shared_ptr<float> speed)
//{
//	worldSpeed = speed;
//}
//
//void Spaceship::setWorldSpeed(float val)
//{
//	*worldSpeed = val;
//}
//
//float Spaceship::getWorldSpeed()const
//{
//	return *worldSpeed;
//}

void Spaceship::dealDamage(float val)
{
	StandardGameObject::dealDamage(val);

	if (currShieldValue > 0.0f)
	{
		currShieldValue -= val;
		if (currShieldValue < 0.0f)
		{
			currLifeValue += currShieldValue;
			currShieldValue = 0.0f;
		}
	}
	else if(currLifeValue > 0.0f) {
		(*hitColor) = glm::vec3(1.0f, 0.42f, 0.2f);
		currLifeValue -= val;
		if (currLifeValue < 0.0f)
			currLifeValue = 0.0f;
	}

	(*guiLifeStatusBarFrac) = currLifeValue / maxLifeValue;
	(*guiShieldStatuBarFrac) = currShieldValue / maxShieldValue;

	if (currLifeValue <= 0.0f)
		*endGameFlag = true;
}

void Spaceship::addScore(int val)
{
	pointsScore += val;
	(*guiPointsScore) = pointsScore;
}

void Spaceship::restart()
{
	transform.setPosition(glm::vec3(0.0f));
	transform.setRotation(glm::vec3(0.0f));
	
	currShieldValue = maxShieldValue;
	currLifeValue = maxLifeValue;
	pointsScore = 0;

	*(guiShieldStatuBarFrac) = 1.0f;
	*(guiLifeStatusBarFrac) = 1.0f;
	*(guiPointsScore) = 0;

	(*hitColor) = glm::vec3(0.4f, 0.4f, 1.0f);

	initClearLaserShotsBuffer();

	beamsBuffer.clear();

	std::shared_ptr<Property<GameObject> > prop = getProperty("hit_detection");
	if (prop->isUsable())
	{
		LaserBeamHitDetection& hitDetect = static_cast<LaserBeamHitDetection&>(*prop);
		hitDetect.initCleanLaserBeamsBuffer();
		hitDetect.initCleanCollisionDataBuffer();
	}
}

std::shared_ptr<float> Spaceship::getShieldFracValuePtr()const
{
	return guiShieldStatuBarFrac;
}

std::shared_ptr<float> Spaceship::getLifeFracValuePtr()const
{
	return guiLifeStatusBarFrac;
}

std::shared_ptr<int> Spaceship::getPointsScorePtr()const
{
	return guiPointsScore;
}

int Spaceship::getPointsScore()const
{
	return pointsScore;
}

void Spaceship::registerEndGameFlag(std::shared_ptr<bool> flag)
{
	endGameFlag = flag;
}

#include "spaceship.h"

Spaceship::Spaceship()
{}

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

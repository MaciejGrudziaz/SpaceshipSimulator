#include "spaceshipgun.h"

void SpaceshipGun::load(const SpaceshipData& data)
{
	this->textureAvailable = data.textureAvailable;

	if (textureAvailable)
		renderer = std::make_shared<TextureRenderObject>();
	else renderer = std::make_shared<RenderObject>();

	renderer->deepCopy(*data.renderer);
	copyModelUniform();

	initBuffer();

	if (textureAvailable)
		loadTextureBufferData(data);
	else loadStandardBufferData(data);

	models.insert(ModelMap::value_type(data.model->name, data.model));
}

void SpaceshipGun::initBuffer()
{
	renderer->init();
}

void SpaceshipGun::loadStandardBufferData(const SpaceshipData& data)
{
	std::vector<float> buffer;

	BasicObjectPtr model = data.model;

	if (model->isUsable())
	{
		int jointIdx = data.jointIdx;
		std::for_each(model->faces.begin(), model->faces.end(), [&model, &buffer, jointIdx](auto face) {
			bool isBody = true;
			for (int i = 0; i < 3; ++i)
			{
				isBody = true;
				for (const BlendingIndexWeightPair& blendInfo : model->controlPoints[face.indices[i]].blendingInfo)
				{
					if (blendInfo.jointIndex != jointIdx && blendInfo.weight > 0.5f)
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

void SpaceshipGun::loadTextureBufferData(const SpaceshipData& data)
{
	std::vector<float> buffer;

	BasicObjectPtr model = data.model;

	if (model->isUsable())
	{
		int jointIdx = data.jointIdx;
		std::for_each(model->faces.begin(), model->faces.end(), [&model, &buffer, jointIdx](auto face) {
			bool isBody = true;
			for (int i = 0; i < 3; ++i)
			{
				isBody = true;
				for (const BlendingIndexWeightPair& blendInfo : model->controlPoints[face.indices[i]].blendingInfo)
				{
					if (blendInfo.jointIndex != jointIdx && blendInfo.weight > 0.5f)
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

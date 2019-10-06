#pragma once

#include <glm/glm.hpp>
#include <memory>
#include <GameRenderer/uniformdata.h>

struct ParticleSystemUniforms
{
	ConstMat4Ptr view;
	ConstMat4Ptr projection;
	ConstVec3Ptr cameraUp;
	ConstVec3Ptr cameraRight;
};

struct ParticleSystemFiles
{
	std::string particleTexture;
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;
};

struct ParticleSystemData
{
	std::shared_ptr<glm::mat4> modelTransform;
	std::shared_ptr<int> continuous;
	std::shared_ptr<float> runTime;
	std::shared_ptr<glm::vec3> baseColor;
	std::shared_ptr<glm::vec3> destColor;
	std::shared_ptr<float> shutDownTime;

	bool launchFlag;
	bool shutingDownFlag;
	float maxLifeTime;

	ParticleSystemData()
		: launchFlag(false)
		, shutingDownFlag(false)
		, maxLifeTime(0.0f)
	{}

	void init()
	{
		modelTransform = std::make_shared<glm::mat4>(1.0f);
		continuous = std::make_shared<int>(1.0f);
		runTime = std::make_shared<float>(0.0f);
		baseColor = std::make_shared<glm::vec3>(0.0f);
		destColor = std::make_shared<glm::vec3>(0.0f);
		shutDownTime = std::make_shared<float>(0.0f);
	}
};

typedef std::shared_ptr<ParticleSystemData> ParticleSystemDataPtr;

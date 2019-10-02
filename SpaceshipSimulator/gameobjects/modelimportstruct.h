#pragma once

#include <string>
#include <GameRenderer/uniformdata.h>

struct ModelData
{
	std::string modelFilename;
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;
	std::string textureFilename;
};

struct ModelExternalUniforms
{
	ConstMat4Ptr view;
	ConstMat4Ptr projection;
};

struct ParticleSystemUniforms
{
	ConstMat4Ptr view;
	ConstMat4Ptr projection;
	ConstVec3Ptr cameraUp;
	ConstVec3Ptr cameraRight;
};

struct HitboxObjectData
{
	std::string hitboxFilename;
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;
};

struct ParticleSystemData
{
	std::string particleTexture;
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;
};


#pragma once

#include <GameResources/gameobjects/gameobject.h>
#include <GameResources/files/mgrimportfile.h>
#include <GameRenderer/texturerenderobject.h>

struct SpaceshipData
{
	std::string modelFilename;
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;
	std::string textureFilename;
};

struct SpaceshipUniforms
{
	ConstMat4Ptr view;
	ConstMat4Ptr projection;
};

class Spaceship: public GameObject
{
public:
	enum error { NO_ERROR, FILE_DATA_ERROR };

	Spaceship();

	void load(const SpaceshipData& data, const SpaceshipUniforms& uniforms);

	std::shared_ptr<RenderObject> getRenderer()const;

	error getErrorCode();

private:
	std::shared_ptr<RenderObject> renderer;
	std::shared_ptr<Hitbox> mainHitbox;
	std::shared_ptr<glm::mat4> modelTransform;

	std::stack<error> errorCode;

	friend class SpaceshipLoadData;
	friend class CleanSpaceshipLoadData;
	friend class TextureSpaceshipLoadData;
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;

class SpaceshipLoadData
{
public:
	SpaceshipLoadData(Spaceship& spaceship, const SpaceshipData& data, const SpaceshipUniforms& uniforms);
	virtual ~SpaceshipLoadData()
	{}

	virtual void load() = 0;

protected:
	void loadModel();
	virtual void loadShaders() = 0;
	void initUniforms();
	virtual void initAttribPointers() = 0;
	virtual void loadBufferData() = 0;

	Spaceship& spaceship;
	SpaceshipData loadFiles;
	SpaceshipUniforms uniforms;
};

class CleanSpaceshipLoadData : public SpaceshipLoadData
{
public:
	CleanSpaceshipLoadData(Spaceship& spaceship, const SpaceshipData& data, const SpaceshipUniforms& uniforms);
	
	void load()override;

protected:
	void loadShaders()override;
	void initAttribPointers()override;
	void loadBufferData()override;
};

class TextureSpaceshipLoadData : public SpaceshipLoadData
{
public:
	TextureSpaceshipLoadData(Spaceship& spaceship, const SpaceshipData& data, const SpaceshipUniforms& uniforms);

	void load()override;

protected:
	void loadShaders()override;
	void initAttribPointers()override;
	void loadBufferData()override;
	void loadTexture();
};

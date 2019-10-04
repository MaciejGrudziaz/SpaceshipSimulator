#pragma once

#include <GameResources/gameobjects/camera.h>
#include "standardgameobject.h"
#include "spaceshipgun.h"
#include "laserbeam.h"
#include "../renderers/beamsrenderer.h"
#include "../renderers/texturebeamsrenderer.h"

class Spaceship: public StandardGameObject
{
public:
	Spaceship();

	virtual void init()override;
	virtual void process()override;

	void loadGuns();

	void addLaserBeam(LaserBeamPtr beam);

	void registerCamera(CameraPtr camera);
	void registerProjectionMat(ConstMat4Ptr projection);

	CameraPtr getCamera()const;
	ConstMat4Ptr getProjectionMat()const;

	TextureBeamsRendererPtr getBeamsRenderer()const;

	void setInputMoveVec(const glm::vec3& val);
	glm::vec3 getInputMoveVec()const;

	void setInputRotation(float val);
	float getInputRotation()const;

	void registerWorldSpeed(std::shared_ptr<float> speed);
	void setWorldSpeed(float val);
	float getWorldSpeed()const;

private:
	void loadStandardBufferData()override;
	void loadTextureBufferData()override;

	std::vector<LaserBeamPtr> laserShots;
	std::vector<float> beamsBuffer;
	TextureBeamsRendererPtr beamsRenderer;
	std::shared_ptr<glm::vec4> beamsColor;
	std::shared_ptr<glm::vec2> beamsSize;

	CameraPtr camera;
	ConstMat4Ptr projectionMat;
	glm::vec3 inputMoveVec;
	float inputRotation;
	std::shared_ptr<float> worldSpeed;

	float maneuverEngineThrust;
	float mainEngineThrust;

	void updateBeamsBuffer();
	void loadBeamsUniforms();
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;

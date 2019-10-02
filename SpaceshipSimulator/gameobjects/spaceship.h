#pragma once

#include <GameResources/gameobjects/camera.h>
#include "standardgameobject.h"
#include "spaceshipgun.h"
#include "laserbeam.h"
#include "../renderers/beamsrenderer.h"

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

	BeamsRendererPtr getBeamsRenderer()const;

private:
	void loadStandardBufferData()override;
	void loadTextureBufferData()override;

	std::vector<LaserBeamPtr> laserShots;
	std::vector<float> beamsBuffer;
	BeamsRendererPtr beamsRenderer;
	std::shared_ptr<glm::vec3> beamsColor;

	CameraPtr camera;
	ConstMat4Ptr projectionMat;

	void updateBeamsBuffer();
	void loadUniforms();
	void loadAttribPointers();
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;

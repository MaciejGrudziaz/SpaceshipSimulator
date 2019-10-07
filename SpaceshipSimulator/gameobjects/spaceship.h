#pragma once

#include <GameResources/gameobjects/camera.h>
#include "../gamelogic/collisions.h"
#include "../gamelogic/laserbeamhitdetection.h"
#include "standardgameobject.h"
#include "spaceshipgun.h"
#include "laserbeam.h"
#include "../renderers/laserrenderer.h"
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

	void dealDamage(float val)override;
	void addScore(int val);
	void restart()override;

	std::shared_ptr<float> getShieldFracValuePtr()const;
	std::shared_ptr<float> getLifeFracValuePtr()const;
	std::shared_ptr<int> getPointsScorePtr()const;
	int getPointsScore()const;

	void registerEndGameFlag(std::shared_ptr<bool> flag);

	float getManeuverEngineThrust()const { return maneuverEngineThrust; }
	float getMainEngineThrust()const { return mainEngineThrust; }
	float getMass()const { return mass; }
	float getManeuverEngineForceVecLength()const { return maneuverEngineForceVecLength; }
	bool getAdvancedFlightMode()const { return advancedFlightMode; }
	float getShotDamage()const { return shotDamage; }
	float getFireRate()const { return fireRate; }
	float getLaserBeamSpeed()const { return laserBeamSpeed; }
	bool getMouseInput()const { return mouseInput;  }

	void setManeuverEngineThrust(float val) { maneuverEngineThrust = val; }
	void setMainEngineThrust(float val) { mainEngineThrust = val; }
	void setMaxShieldValue(float val) { maxShieldValue = val; currShieldValue = val; }
	void setMaxLifeValue(float val) { maxLifeValue = val; currLifeValue = val; }
	void setMass(float val) { mass = val; }
	void setManeuverEngineForceVecLength(float val) { maneuverEngineForceVecLength = val; }
	void setAdvancedFlightMode(bool val) { advancedFlightMode = val; }
	void setShotDamage(float val) { shotDamage = val; }
	void setFireRate(float val) { fireRate = val; }
	void setLaserBeamSpeed(float val) { laserBeamSpeed = val; }
	void setMouseInput(bool val) { mouseInput = val; }

private:
	std::shared_ptr<bool> endGameFlag;

	std::vector<LaserBeamPtr> laserShots;
	std::vector<float> beamsBuffer;
	TextureBeamsRendererPtr beamsRenderer;
	std::shared_ptr<glm::vec4> beamsColor;
	std::shared_ptr<glm::vec2> beamsSize;

	CameraPtr camera;
	ConstMat4Ptr projectionMat;
	glm::vec3 inputMoveVec;
	float inputRotation;
	bool mouseInput;

	float maneuverEngineThrust;
	float mainEngineThrust;
	float maxShieldValue;
	float maxLifeValue;
	float mass;
	float maneuverEngineForceVecLength;
	bool advancedFlightMode;
	float shotDamage;
	float fireRate;
	float laserBeamSpeed;

	float currShieldValue;
	float currLifeValue;
	int pointsScore;

	std::shared_ptr<float> guiShieldStatuBarFrac;
	std::shared_ptr<float> guiLifeStatusBarFrac;
	std::shared_ptr<int> guiPointsScore;

	void loadStandardBufferData()override;
	void loadTextureBufferData()override;

	void updateBeamsBuffer();
	//void clearBeamsBuffer();
	void loadBeamsUniforms();

	int findNotActiveLaserShot();
};

typedef std::shared_ptr<Spaceship> SpaceshipPtr;

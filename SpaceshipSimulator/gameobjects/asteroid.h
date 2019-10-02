#pragma once

#include "standardgameobject.h"

class Asteroid: public StandardGameObject
{
public:
	Asteroid();

	void loadViewMatrixPtr(ConstMat4Ptr view);
	void loadProjectionMatrixPtr(ConstMat4Ptr projection);

private:
	ConstMat4Ptr viewPtr;
	ConstMat4Ptr projectionPtr;
};

typedef std::shared_ptr<Asteroid> AsteroidPtr;

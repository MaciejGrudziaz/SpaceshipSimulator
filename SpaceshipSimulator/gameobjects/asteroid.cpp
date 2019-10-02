#include "asteroid.h"

Asteroid::Asteroid()
{}

void Asteroid::loadViewMatrixPtr(ConstMat4Ptr view)
{
	viewPtr = view;
}

void Asteroid::loadProjectionMatrixPtr(ConstMat4Ptr projection)
{
	projectionPtr = projection;
}

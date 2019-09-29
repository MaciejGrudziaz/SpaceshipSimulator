//#include "hitbox.h"
#include <stdafx.h>

unsigned Hitbox::indices[] = {
	0, 1, 1, 2, 2, 3, 3, 0,
	0, 4, 1, 5, 2, 6, 3, 7,
	4, 5, 5, 6, 6, 7, 7, 4
};

Hitbox::Hitbox() 
	:jointIdx(-1)
	, name("")
	, damageMultiplier(1.0)
{ }

void Hitbox::deepCopy(const Hitbox& hitbox)
{
	this->name = hitbox.name;
	this->localAxis = hitbox.localAxis;
	this->jointIdx = hitbox.jointIdx;
	this->damageMultiplier = hitbox.damageMultiplier;
	
	std::for_each(hitbox.controlPointsIdx.begin(), hitbox.controlPointsIdx.end(), [this](auto controlPoint) {
		this->controlPointsIdx.push_back(controlPoint);
	});

	std::memcpy(this->basicVertices, hitbox.basicVertices, 8 * sizeof(glm::vec3));
	std::memcpy(this->transformVertices, hitbox.transformVertices, 8 * sizeof(glm::vec3));
	std::memcpy(this->basicNormals, hitbox.basicNormals, 6 * sizeof(glm::vec3));
	std::memcpy(this->transformNormals, hitbox.transformNormals, 6 * sizeof(glm::vec3));
}

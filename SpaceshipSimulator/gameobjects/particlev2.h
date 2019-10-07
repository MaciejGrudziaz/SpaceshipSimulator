#pragma once

#include <glm/vec3.hpp>
#include <memory>

struct ParticleV2
{
	glm::vec3 pos;
	glm::vec3 speed;
	float lifeTime;
	float size;
	
	ParticleV2()
		: pos(0.0f)
		, speed(0.0f)
		, lifeTime(0.0f)
		, size(0.0f)
	{}
};

typedef std::shared_ptr<ParticleV2> ParticleV2Ptr;


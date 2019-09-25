#include "hitbox.h"

unsigned Hitbox::indices[] = {
	0, 1, 1, 2, 2, 3, 3, 0,
	0, 4, 1, 5, 2, 6, 3, 7,
	4, 5, 5, 6, 6, 7, 7, 4
};

Hitbox::Hitbox() :jointIdx(-1), name(""), damageMultiplier(1.0){ }

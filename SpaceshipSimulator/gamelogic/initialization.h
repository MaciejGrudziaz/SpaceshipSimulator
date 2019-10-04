#pragma once

#include <ctime>
#include "../gameengine.h"
#include "../gameobjects/gameresources.h"

#include "spaceshipinitialization.h"
#include "asteroidinitialization.h"

#include "spaceshipinputcontrol.h"
#include "asteroidbehaviour.h"
#include "collisions.h"
#include "gunbehaviour.h"
#include "spaceshipenginescontrol.h"

#include "../dev/particlesystemtest.h"

#include "../dev/rotation.h"

void setCamera(GameEngine& engine);

void initializeGame(GameEngine& engine);
void processGame(GameEngine& engine);
void invalidateGame(GameEngine& engine);

void loadObjectsCollisionProperty(GameEngine& engine);

void initializeParticlesTest(GameEngine& engine);
void processParticlesTest(GameEngine& engine);
void invalidateParticles(GameEngine& engine);

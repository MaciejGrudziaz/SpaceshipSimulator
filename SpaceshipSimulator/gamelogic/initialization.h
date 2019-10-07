#pragma once

#include <ctime>
#include "../gameengine.h"
#include "../gameobjects/gameresources.h"

#include "spaceshipinitialization.h"
#include "asteroidinitialization.h"

#include "spaceshipinputcontrol.h"
#include "collisions.h"
#include "gunbehaviour.h"
#include "spaceshipenginescontrol.h"

#include "../dev/particlesystemtest.h"

#include "../dev/rotation.h"

void setCamera(GameEngine& engine);
void loadWorldSpeed(GameEngine& engine);

void initializeGame(GameEngine& engine);
void processGame(GameEngine& engine);
void restartGame(GameEngine& engine);
void invalidateGame(GameEngine& engine);

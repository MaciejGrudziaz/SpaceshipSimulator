#pragma once

#include "../gameengine.h"
#include "../gameobjects/gameresources.h"

void loadSpaceship(GameEngine& engine);
void writeToStdOutBonesNames(SpaceshipPtr spaceship);
void addBodyHitboxToSpaceship(GameEngine& engine);
void addWingHitboxToSpaceship(GameEngine& engine);
void addCocpitHitboxToSpaceship(GameEngine& engine);
void addMainEngineHitboxToSpaceship(GameEngine& engine);
void loadGuns(GameEngine& engine);
void loadParticlesSytem(GameEngine& engine);
void registerSpaceshipCollisionSystem(GameEngine& engine);
void linkSpaceshipDataToGui(GameEngine& engine);
void loadSapceshipDataFromConfigFile(GameEngine& engine);

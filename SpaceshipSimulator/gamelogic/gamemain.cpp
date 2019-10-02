#include "gamemain.h"

GAME_INIT(engine)
{
	srand(time(0));
	//initializeGame(engine);
	initializeParticlesTest(engine);
}

GAME_PROCESS(engine)
{
	//processGame(engine);
	processParticlesTest(engine);
}

GAME_END(engine)
{
	invalidateParticles(engine);
}

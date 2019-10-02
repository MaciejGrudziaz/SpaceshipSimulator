#include "gamemain.h"

GAME_INIT(engine)
{
	srand(time(0));
#ifdef MAIN_GAME 
	initializeGame(engine);
#endif // MAIN_GAME 

#ifdef PARTICLES_TEST
	initializeParticlesTest(engine);
#endif // PARTICLES_TEST
}

GAME_PROCESS(engine)
{
#ifdef MAIN_GAME
	processGame(engine);
#endif // MAIN_GAME

#ifdef PARTICLES_TEST
	processParticlesTest(engine);
#endif // PARTICLES_TEST
}

GAME_END(engine)
{
#ifdef MAIN_GAME
	invalidateGame(engine);
#endif // MAIN_GAME

#ifdef PARTICLES_TEST
	invalidateParticles(engine);
#endif // PARTICLES_TEST
}

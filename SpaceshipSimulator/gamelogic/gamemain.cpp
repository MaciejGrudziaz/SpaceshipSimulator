#include "gamemain.h"

GAME_INIT(engine)
{
	initializeGame(engine);
}

GAME_PROCESS(engine)
{
	processGame(engine);
}

GAME_RESTART(engine)
{
	restartGame(engine);
}

GAME_END(engine)
{
	invalidateGame(engine);
}

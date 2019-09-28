#include "gameengine.h"

GameEngine::GameEngine()
	: initializeFun(MG::initialize<GameEngine>)
	, processFun(MG::process<GameEngine>)
	, endFun(MG::end<GameEngine>)
{
	mainWnd = std::make_unique<MainWindow>("Spaceship Simulator ver0.1");
	spaceship = std::make_unique<Spaceship>();
}

void GameEngine::launch()
{

	initializeFun(*this);
}

void GameEngine::process()
{
	processFun(*this);
}

void GameEngine::end()
{
	endFun(*this);
}

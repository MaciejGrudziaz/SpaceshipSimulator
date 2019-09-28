#include "gameinit.h"

template<>
void MG::initialize<GameEngine>(const GameEngine& val)
{
	std::cout << "init!" << std::endl;
}

template<>
void MG::process<GameEngine>(const GameEngine& val)
{
	std::cout << "process!" << std::endl;
}

template<>
void MG::end<GameEngine>(const GameEngine& val)
{
	std::cout << "end!" << std::endl;
}

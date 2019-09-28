#pragma once

#include "mainwindow/mainwindow.h"
#include "utilities/time.h"
#include "gameobjects/spaceship.h"

namespace MG {
	template<class T>
	void initialize(const T& val);

	template<class T>
	void process(const T& val);

	template<class T>
	void end(const T& val);
}

class GameEngine
{
public:
	GameEngine();

	void launch();
	void process();
	void end();

private:
	std::unique_ptr<MainWindow> mainWnd;
	std::function<void(const GameEngine&)> initializeFun;
	std::function<void(const GameEngine&)> processFun;
	std::function<void(const GameEngine&)> endFun;

	std::unique_ptr<Spaceship> spaceship;
};


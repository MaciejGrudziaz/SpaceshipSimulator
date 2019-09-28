#pragma once

#include <GameResources/gameobjects/camera.h>
#include "mainwindow/mainwindow.h"
#include "utilities/time.h"
#include "gameobjects/spaceship.h"

namespace MG {
	template<class T>
	void gameResourcesInit(T& val);

	template<class T>
	void initialize(T& val);

	template<class T>
	void process(T& val);

	template<class T>
	void end(T& val);
}

template<class T>
class GameResources;

class GameEngine
{
public:
	GameEngine();

	void launch();
	void process();
	void end();

	void addRenderer(RenderObjectPtr renderer);
	//void registerSpaceship(SpaceshipPtr spaceship);
	void registerCamera(CameraPtr camera);

	//SpaceshipPtr getSpaceship() const;
	CameraPtr getCamera() const;
	ConstMat4Ptr getProjectionMatPtr() const;

	void registerResources(std::shared_ptr<GameResources<GameEngine> > resources);
	std::shared_ptr<GameResources<GameEngine> > getResources();

private:
	Time mainRefreshLogicTimer;

	std::unique_ptr<MainWindow> mainWnd;
	std::function<void(GameEngine&)> gameResourcesInitFun;
	std::function<void(GameEngine&)> initializeFun;
	std::function<void(GameEngine&)> processFun;
	std::function<void(GameEngine&)> endFun;

	std::shared_ptr<GameResources<GameEngine> > gameResources;

	CameraPtr camera;
	//SpaceshipPtr spaceship;

	std::list<RenderObjectPtr> renderers;

	bool finish;

	void processLogic(int refreshCount);
	void processGraphics();
};


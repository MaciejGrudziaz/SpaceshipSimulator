#pragma once

#include <GameResources/gameobjects/camera.h>
#include "mainwindow/mainwindow.h"
#include "utilities/time.h"
#include "gui/gui.h"
#include "gamemanager.h"

namespace MG {
	template<class T>
	void gameResourcesInit(T& val);

	template<class T>
	void initialize(T& val);

	template<class T>
	void process(T& val);

	template<class T>
	void restart(T& val);

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
	void setCamera(CameraPtr camera);

	CameraPtr getCamera() const;
	ConstMat4Ptr getProjectionMatPtr() const;

	void registerResources(std::shared_ptr<GameResources<GameEngine> > resources);
	std::shared_ptr<GameResources<GameEngine> > getResources();

	GuiPtr getGui()const;

	static int getKey(int key);

	struct CursorPos;
	static CursorPos getCursorPos();

	std::shared_ptr<bool> getEndGameFlag()const;
	void registerPointScore(int score);
	GameConfigData getConfigData()const;

	bool getShowHitboxesFlag()const;

private:
	Time mainRefreshLogicTimer;

	static std::unique_ptr<MainWindow> mainWnd;
	std::function<void(GameEngine&)> gameResourcesInitFun;
	std::function<void(GameEngine&)> initializeFun;
	std::function<void(GameEngine&)> processFun;
	std::function<void(GameEngine&)> restartFun;
	std::function<void(GameEngine&)> endFun;

	std::shared_ptr<GameResources<GameEngine> > gameResources;

	CameraPtr camera;

	std::list<RenderObjectPtr> renderers;

	GuiPtr gui;

	GameManager gameManager;

	bool finish;

	void processLogic(int refreshCount);
	void processGraphics();

	void checkEvents();
	void setupMouseInputMode();
};

struct GameEngine::CursorPos
{
	double x, y;

	CursorPos operator-(const CursorPos& pos)
	{
		CursorPos ret;
		ret.x = this->x - pos.x;
		ret.y = this->y - pos.y;
		return ret;
	}

	CursorPos operator+(const CursorPos& pos)
	{
		CursorPos ret;
		ret.x = this->x + pos.x;
		ret.y = this->y + pos.y;
		return ret;
	}

};


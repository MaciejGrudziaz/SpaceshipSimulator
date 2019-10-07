#include "gameengine.h"

std::unique_ptr<MainWindow> GameEngine::mainWnd = std::make_unique<MainWindow>("Spaceship Simulator ver0.1");

GameEngine::GameEngine()
	: gameResourcesInitFun(MG::gameResourcesInit<GameEngine>)
	, initializeFun(MG::initialize<GameEngine>)
	, processFun(MG::process<GameEngine>)
	, restartFun(MG::restart<GameEngine>)
	, endFun(MG::end<GameEngine>)
	, finish(false)
	, gui(std::make_shared<Gui>())
{}

void GameEngine::launch()
{
	glfwWindowHint(GLFW_SAMPLES, 16);
	mainWnd->create();

	setupMouseInputMode();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	mainRefreshLogicTimer.setTimer(std::make_shared<TimerOptions<Hz> >(60.0f, BasicTimerOptions::continuous), 
		std::bind(&GameEngine::processLogic, this, std::placeholders::_1));

	gui->initialize(mainWnd->getGLFWwindow());
	gui->resize(mainWnd->getWidth(), mainWnd->getHeight());
	gui->renderLaodingScreen(mainWnd->getGLFWwindow());
	gui->registerHighScoreValue(gameManager.getHighScorePtr());

	gameManager.intitializeGameData();
	if (gameManager.getErrorCode() != GameManager::NO_ERROR)
	{
		if (gameManager.getErrorCode() == GameManager::CONFIG_FILE_NOT_AVAILABLE)
			gui->setErrorInitMsg("File config.cfg not found!");
		else gui->setErrorInitMsg("File " + gameManager.getNotFoundResourceFilename() + " not found!");
	}
	else {
		gameResourcesInitFun(*this);
		initializeFun(*this);

		gameManager.launchMainMenu();
	}
}

void GameEngine::setupMouseInputMode()
{
	glfwSetInputMode(mainWnd->getGLFWwindow(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	if (glfwRawMouseMotionSupported())
		glfwSetInputMode(mainWnd->getGLFWwindow(), GLFW_RAW_MOUSE_MOTION, GLFW_TRUE);
}

void GameEngine::process()
{
	while (!finish)
	{
		Time::refreshTimers();

		//processGraphics();

		checkEvents();
	}
}

void GameEngine::checkEvents()
{
	if (getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
	{
		restartFun(*this);
		finish = true;
	}

	if (gameManager.getGameState() == GameManager::MAIN_MENU)
	{
		if (getKey(GLFW_KEY_SPACE) == GLFW_PRESS)
		{
			gui->launchMainGame();
			gameManager.launchMainGame();
		}
	}

	if (gameManager.getGameState() == GameManager::END_GAME)
	{
		if (getKey(GLFW_KEY_R) == GLFW_PRESS)
		{
			restartFun(*this);
			gameManager.launchMainGame();
			gui->launchMainGame();
		}
	}

	gameManager.checkEndGame();
	if (gameManager.getGameState() == GameManager::END_GAME)
		gui->setEndGameMode();
}

void GameEngine::processLogic(int refreshCount)
{
	if (gameManager.getGameState() == GameManager::GAME)
	{
		processFun(*this);
	}
	gui->process();

	processGraphics();
}

void GameEngine::processGraphics()
{
	if (gameManager.getGameState() == GameManager::GAME || gameManager.getGameState() == GameManager::END_GAME)
	{
		glViewport(0, 0, mainWnd->getWidth(), mainWnd->getHeight());
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		if (camera)
			camera->update();

		std::for_each(renderers.begin(), renderers.end(), [](auto renderer) {
			renderer->process();
		});

		gui->render();
		gui->resize(mainWnd->getWidth(), mainWnd->getHeight());

		glfwSwapBuffers(mainWnd->getGLFWwindow());
		glfwPollEvents();
	}
	else if (gameManager.getGameState() == GameManager::MAIN_MENU)
		gui->mainMenu(mainWnd->getGLFWwindow());
	else if (gameManager.getGameState() == GameManager::CRASH)
		gui->renderInitErrorScreen(mainWnd->getGLFWwindow());
}

void GameEngine::end()
{
	if(gameManager.getGameState() != GameManager::CRASH)
		endFun(*this);
	gui->invalidate();
	mainWnd->destory();
}

void GameEngine::addRenderer(RenderObjectPtr renderer)
{
	renderers.push_back(renderer);
}

void GameEngine::setCamera(CameraPtr camera)
{
	this->camera = camera;
}

CameraPtr GameEngine::getCamera() const
{
	return camera;
}

ConstMat4Ptr GameEngine::getProjectionMatPtr() const
{
	return mainWnd->getProjectionPtr();
}

void GameEngine::registerResources(std::shared_ptr<GameResources<GameEngine> > resources)
{
	gameResources = resources;
}

std::shared_ptr<GameResources<GameEngine> > GameEngine::getResources()
{
	return gameResources;
}

GuiPtr GameEngine::getGui()const
{
	return gui;
}

int GameEngine::getKey(int key)
{
	return glfwGetKey(mainWnd->getGLFWwindow(), key);
}

GameEngine::CursorPos GameEngine::getCursorPos()
{
	CursorPos pos;
	glfwGetCursorPos(mainWnd->getGLFWwindow(), &pos.x, &pos.y);

	return pos;
}

std::shared_ptr<bool> GameEngine::getEndGameFlag()const
{
	return gameManager.getEndGameFlag();
}

void GameEngine::registerPointScore(int score)
{
	gameManager.registerLastScore(score);
}

GameConfigData GameEngine::getConfigData()const
{
	return gameManager.getConfigData();
}

bool GameEngine::getShowHitboxesFlag()const
{
	return gameManager.getConfigData().showHitboxes;
}

#include "gameengine.h"

std::unique_ptr<MainWindow> GameEngine::mainWnd = std::make_unique<MainWindow>("Spaceship Simulator ver0.1");

GameEngine::GameEngine()
	: gameResourcesInitFun(MG::gameResourcesInit<GameEngine>)
	, initializeFun(MG::initialize<GameEngine>)
	, processFun(MG::process<GameEngine>)
	, endFun(MG::end<GameEngine>)
	, finish(false)
{}

void GameEngine::launch()
{
	glfwWindowHint(GLFW_SAMPLES, 16);

	mainWnd->setFov(45.0f);
	mainWnd->create();

	setupMouseInputMode();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	mainRefreshLogicTimer.setTimer(std::make_shared<TimerOptions<Hz> >(60.0f, BasicTimerOptions::continuous), 
		std::bind(&GameEngine::processLogic, this, std::placeholders::_1));

	gameResourcesInitFun(*this);
	initializeFun(*this);
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

		processGraphics();

		checkCloseEvent();
	}
}

void GameEngine::checkCloseEvent()
{
	if (getKey(GLFW_KEY_ESCAPE) == GLFW_PRESS)
		finish = true;
}

void GameEngine::processLogic(int refreshCount)
{
	processFun(*this);
}

void GameEngine::processGraphics()
{
	glViewport(0, 0, mainWnd->getWidth(), mainWnd->getHeight());
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if(camera)
		camera->update();

	std::for_each(renderers.begin(), renderers.end(), [](auto renderer) {
		renderer->process();
	});

	glfwSwapBuffers(mainWnd->getGLFWwindow());
	glfwPollEvents();
}

void GameEngine::end()
{
	endFun(*this);
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

#include "gameengine.h"

GameEngine::GameEngine()
	: gameResourcesInitFun(MG::gameResourcesInit<GameEngine>)
	, initializeFun(MG::initialize<GameEngine>)
	, processFun(MG::process<GameEngine>)
	, endFun(MG::end<GameEngine>)
	//, gameResources(std::make_shared<GameResources<GameEngine> >())
	, finish(false)
{
	mainWnd = std::make_unique<MainWindow>("Spaceship Simulator ver0.1");
}

void GameEngine::launch()
{
	mainWnd->create();

	glEnable(GL_MULTISAMPLE);
	glEnable(GL_DEPTH_TEST);

	mainRefreshLogicTimer.setTimer(std::make_shared<TimerOptions<Hz> >(60.0f, BasicTimerOptions::continuous), 
		std::bind(&GameEngine::processLogic, this, std::placeholders::_1));

	gameResourcesInitFun(*this);
	initializeFun(*this);
}

void GameEngine::process()
{
	while (!finish)
	{
		Time::refreshTimers();

		processGraphics();
	}
}

void GameEngine::processLogic(int refreshCount)
{
	//for (int i = 0; i < refreshCount; ++i)
	//{
	//	if(spaceship)
	//		spaceship->process();
	//}

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
	mainWnd->destory();
	endFun(*this);
}

void GameEngine::addRenderer(RenderObjectPtr renderer)
{
	renderers.push_back(renderer);
}

//void GameEngine::registerSpaceship(SpaceshipPtr spaceship)
//{
//	this->spaceship = spaceship;
//}

void GameEngine::registerCamera(CameraPtr camera)
{
	this->camera = camera;
}

//SpaceshipPtr GameEngine::getSpaceship() const
//{
//	return spaceship;
//}

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

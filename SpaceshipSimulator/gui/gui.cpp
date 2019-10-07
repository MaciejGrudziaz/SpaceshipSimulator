#include "gui.h"

Gui::Gui()
	: wndWidth(0)
	, wndHeight(0)
{
	shieldRenderer = std::make_shared<StatusBarRenderer>();
	lifeRenderer = std::make_shared<StatusBarRenderer>();
}

void Gui::initialize(GLFWwindow* wnd)
{
	shieldRenderer->initialize("shaders/statusBar.vert", "shaders/statusBar.frag");
	shieldRenderer->setActive(true);
	shieldRenderer->setFrac(1.0f);
	shieldRenderer->setMaxColor(glm::vec3(0.2f, 0.2f, 1.0f));
	shieldRenderer->setMinColor(glm::vec3(0.8f, 0.8f, 1.0f));
	shieldRenderer->setMaxPos(glm::vec2(0.75f, 0.065f));
	shieldRenderer->setMinPos(glm::vec2(0.25f, 0.05f));

	lifeRenderer->initialize("shaders/statusBar.vert", "shaders/statusBar.frag");
	lifeRenderer->setActive(true);
	lifeRenderer->setFrac(1.0f);
	lifeRenderer->setMaxColor(glm::vec3(0.4f, 1.0f, 0.4f));
	lifeRenderer->setMinColor(glm::vec3(1.0f, 0.0f, 0.0f));
	lifeRenderer->setMaxPos(glm::vec2(0.75f, 0.04f));
	lifeRenderer->setMinPos(glm::vec2(0.25f, 0.025f));

	scoreIncColor = glm::vec3(1.0f, 0.8f, 0.0f);
	scoreColor = glm::vec3(1.0f, 1.0f, 1.0f);
	prevScoreVal = 0;

	scoreUpdateAnimMaxTime = 0.8f;
	scoreUpdateAnimCurrTime = 0.0f;

	spaceshipMenuRotSpeed = 15.0f;
	menuBeginTextAnimCurrTime = 0.0f;
	menuBeginTextAnimMaxTime = 1.0f;
	beginTextColorAnimDir = 1.0f;

	gltInit();
}

void Gui::renderLaodingScreen(GLFWwindow* wnd)
{
	currState = INIT;

	glViewport(0, 0, wndWidth, wndHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLTtext *loadText = gltCreateText();
	std::string str = "loading...";
	gltSetText(loadText, str.c_str());

	gltBeginDraw();

	gltColor(scoreColor.x, scoreColor.y, scoreColor.z, 1.0f);
	gltDrawText2D(loadText, 0.35f * static_cast<float>(wndWidth), 0.45 * static_cast<float>(wndHeight), 5.0f);

	gltEndDraw();

	gltDeleteText(loadText);

	glfwSwapBuffers(wnd);
}

void Gui::renderInitErrorScreen(GLFWwindow* wnd)
{
	currState = INIT;

	glViewport(0, 0, wndWidth, wndHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	
	GLTtext *errorText = gltCreateText();
	std::string str = initErrorMsg;
	gltSetText(errorText, str.c_str());

	gltBeginDraw();

	gltColor(scoreColor.x, scoreColor.y, scoreColor.z, 1.0f);
	gltDrawText2D(errorText, 0.225f * static_cast<float>(wndWidth), 0.45 * static_cast<float>(wndHeight), 2.0f);

	gltEndDraw();

	gltDeleteText(errorText);

	glfwSwapBuffers(wnd);
	glfwPollEvents();
}

void Gui::mainMenu(GLFWwindow* wnd)
{
	currState = MAIN_MENU;

	glViewport(0, 0, wndWidth, wndHeight);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	camera->getTransform().setPosition(glm::vec3(0.0f, 3.0f, -10.0f));
	camera->getTransform().setRotation(glm::vec3(-15.0f, 180.0f, 0.0f));
	camera->update();

	renderSpaceship();

	GLTtext *titleText = gltCreateText();
	std::string str = "ASTEROIDS 2K19";
	gltSetText(titleText, str.c_str());

	GLTtext *beginText = gltCreateText();
	str = "PRESS SPACE TO BEGIN";
	gltSetText(beginText, str.c_str());

	gltBeginDraw();

	gltColor(1.0f, 1.0f, 1.0f, 1.0f);
	gltDrawText2D(titleText, 0.225f * static_cast<float>(wndWidth), 0.1 * static_cast<float>(wndHeight), 6.0f);

	float colorFrac = menuBeginTextAnimCurrTime / menuBeginTextAnimMaxTime;
	if (colorFrac < 0.0f) colorFrac = 0.0f;
	if (colorFrac > 1.0f) colorFrac = 1.0f;

	glm::vec3 beginTextColor = glm::vec3(0.0f, 0.0f, 0.0f) * colorFrac + glm::vec3(1.0f, 1.0f, 1.0f) * (1.0f - colorFrac);
	gltColor(beginTextColor.x, beginTextColor.y, beginTextColor.z, 1.0f);
	gltDrawText2D(beginText, 0.3f * static_cast<float>(wndWidth), 0.85 * static_cast<float>(wndHeight), 3.0f);

	gltEndDraw();

	gltDeleteText(titleText);

	glfwSwapBuffers(wnd);
	glfwPollEvents();
}

void Gui::launchMainGame()
{
	currState = GAME;

	spaceship->getTransform().setPosition(defaultSpaceshipPos);
	spaceship->getTransform().setRotation(defaultSpaceshipRot);
	spaceship->setPropertiesFlag(true);

	camera->getTransform().setRotation(glm::vec3(0.0f));
	camera->getTransform().setPosition(defaultCameraPos);
	camera->update();
}

void Gui::setEndGameMode()
{
	currState = END_GAME;
}

void Gui::renderSpaceship()
{
	spaceship->getRenderer()->process();
	
	GameObjectPtr obj = spaceship->getChild("gun_left");
	if (obj->isUsable())
	{
		static_cast<StandardGameObject&>(*obj).getRenderer()->process();
	}
	obj = spaceship->getChild("gun_right");
	if (obj->isUsable())
	{
		static_cast<StandardGameObject&>(*obj).getRenderer()->process();
	}
}

void Gui::process()
{
	if (currState == GAME)
	{
		if (scoreUpdateAnimCurrTime < scoreUpdateAnimMaxTime)
		{
			scoreUpdateAnimCurrTime += Time::deltaTime;
		}

		if (prevScoreVal != (*pointsScore))
		{
			scoreUpdateAnimCurrTime = 0.0f;
		}
		float frac = scoreUpdateAnimCurrTime / scoreUpdateAnimMaxTime;
		if (frac > 1.0f) frac = 1.0f;

		scoreColor = glm::vec3(1.0f, 1.0f, 1.0f) * frac + scoreIncColor * (1.0f - frac);

		prevScoreVal = (*pointsScore);
	}
	else if (currState == MAIN_MENU)
	{
		spaceship->getTransform().setPosition(glm::vec3(0.0f, 0.0f, 0.0f));

		glm::vec3 spaceshipRot = spaceship->getTransform().getRotation();
		spaceshipRot.x = -90.0f;
		spaceshipRot.y += spaceshipMenuRotSpeed * Time::deltaTime;
		if (spaceshipRot.y > 360.0f) spaceshipRot.y -= 360.0f;
		spaceship->getTransform().setRotation(spaceshipRot);

		spaceship->setPropertiesFlag(false);
		spaceship->process();

		if (menuBeginTextAnimCurrTime > menuBeginTextAnimMaxTime && beginTextColorAnimDir > 0.0f)
			beginTextColorAnimDir = -1.0f;
		if (menuBeginTextAnimCurrTime < 0.0f && beginTextColorAnimDir < 0.0f)
			beginTextColorAnimDir = 1.0f;

		menuBeginTextAnimCurrTime += Time::deltaTime * beginTextColorAnimDir;
	}
}

void Gui::render()
{
	renderInGameScreen();

	if (currState == END_GAME)
	{
		renderEndGameScreen();
	}
}

void Gui::renderInGameScreen()
{
	updateSatusBars();
	shieldRenderer->process();
	lifeRenderer->process();

	GLTtext *scoreText = gltCreateText();
	std::string str = "Score: " + std::to_string(*pointsScore);
	gltSetText(scoreText, str.c_str());

	GLTtext *highScoreText = gltCreateText();
	str = "High score: " + std::to_string(*highScore);
	gltSetText(highScoreText, str.c_str());

	gltBeginDraw();

	gltColor(scoreColor.x, scoreColor.y, scoreColor.z, 1.0f);
	gltDrawText2D(scoreText, 0.015f * static_cast<float>(wndWidth), 0.935f * static_cast<float>(wndHeight), 2.0f);

	gltColor(1.0f, 1.0f, 1.0f, 1.0f);
	gltDrawText2D(highScoreText, 0.765f * static_cast<float>(wndWidth), 0.935f * static_cast<float>(wndHeight), 1.5f);

	gltEndDraw();

	gltDeleteText(scoreText);
	gltDeleteText(highScoreText);
}

void Gui::renderEndGameScreen()
{
	updateSatusBars();
	shieldRenderer->process();
	lifeRenderer->process();

	bool newHighScoreFlag = (*pointsScore > *highScore) ? true : false;

	GLTtext *highScoreText = gltCreateText();
	std::string str;
	if (newHighScoreFlag)
	{
		str = "NEW HIGH SCORE!";
		gltSetText(highScoreText, str.c_str());
	}

	GLTtext *endText = gltCreateText();
	str = "GAME OVER!";
	gltSetText(endText, str.c_str());

	GLTtext *retryText = gltCreateText();
	str = "press R to try again";
	gltSetText(retryText, str.c_str());

	GLTtext *exitText = gltCreateText();
	str = "press ESC to exit";
	gltSetText(exitText, str.c_str());

	gltBeginDraw();

	if (newHighScoreFlag)
	{
		gltColor(0.45f, 1.0f, 0.27f, 1.0f);
		gltDrawText2D(highScoreText, 0.387f * static_cast<float>(wndWidth), 0.3f * static_cast<float>(wndHeight), 3.0f);
	}

	gltColor(1.0f, 0.8f, 0.0f, 1.0f);
	gltDrawText2D(endText, 0.45f * static_cast<float>(wndWidth), 0.4f * static_cast<float>(wndHeight), 2.0f);

	gltColor(1.0f, 1.0f, 1.0f, 1.0f);
	gltDrawText2D(retryText, 0.425f * static_cast<float>(wndWidth), 0.5f * static_cast<float>(wndHeight), 1.5f);

	gltColor(1.0f, 1.0f, 1.0f, 1.0f);
	gltDrawText2D(exitText, 0.433f * static_cast<float>(wndWidth), 0.55f * static_cast<float>(wndHeight), 1.5f);

	gltEndDraw();

	gltDeleteText(highScoreText);
	gltDeleteText(endText);
	gltDeleteText(retryText);
	gltDeleteText(exitText);
}

void Gui::updateSatusBars()
{
	if (shieldFracValue)
		shieldRenderer->setFrac(*shieldFracValue);
	if (lifeFracValue)
		lifeRenderer->setFrac(*lifeFracValue);
}

void Gui::invalidate()
{
	gltTerminate();
	shieldRenderer->invalidate();
	lifeRenderer->invalidate();
}

void Gui::resize(int width, int height)
{
	if (width != wndWidth || height != wndHeight)
	{
		wndWidth = width;
		wndHeight = height;
		gltViewport(width, height);
	}
}

void Gui::registerLifeFracValue(std::shared_ptr<float> life)
{
	lifeFracValue = life;
}

void Gui::registerShieldFracValue(std::shared_ptr<float> shield)
{
	shieldFracValue = shield;
}

void Gui::registerPointsCounter(std::shared_ptr<int> points)
{
	pointsScore = points;
}

void Gui::registerHighScoreValue(std::shared_ptr<int> highScore)
{
	this->highScore = highScore;
}

void Gui::registerSpaceship(SpaceshipPtr spaceship)
{
	this->spaceship = spaceship;
	defaultSpaceshipPos = spaceship->getTransform().getPosition();
	defaultSpaceshipRot = spaceship->getTransform().getRotationQuat();
}

void Gui::registerCamera(CameraPtr camera)
{
	this->camera = camera;
	defaultCameraPos = camera->getTransform().getPosition();
}

void Gui::setErrorInitMsg(const std::string& msg) 
{
	initErrorMsg = msg;
}

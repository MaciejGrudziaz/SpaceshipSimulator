#pragma once

#include <memory>
#include "../renderers/statusbarrenderer.h"
#include "../utilities/time.h"
#include <GLFW/glfw3.h>
#include "../gameobjects/spaceship.h"

#define GLT_IMPLEMENTATION
#include "../renderers/gltext.h"

class Gui
{
public:
	enum GameState {INIT, MAIN_MENU, GAME, END_GAME};

	Gui();

	void initialize(GLFWwindow* wnd);
	void renderLaodingScreen(GLFWwindow* wnd);
	void renderInitErrorScreen(GLFWwindow* wnd);
	void mainMenu(GLFWwindow* wnd);
	void launchMainGame();
	void setEndGameMode();

	void process();

	void render();

	void invalidate();

	void resize(int width, int height);

	void registerLifeFracValue(std::shared_ptr<float> life);
	void registerShieldFracValue(std::shared_ptr<float> shield);
	void registerPointsCounter(std::shared_ptr<int> points);
	void registerHighScoreValue(std::shared_ptr<int> highScore);
	void registerSpaceship(SpaceshipPtr spaceship);
	void registerCamera(CameraPtr camera);

	void setErrorInitMsg(const std::string& msg);

private:
	GameState currState;
	int wndWidth, wndHeight;

	std::string initErrorMsg;

	glm::vec3 scoreIncColor;
	glm::vec3 scoreColor;

	SpaceshipPtr spaceship;
	glm::vec3 defaultSpaceshipPos;
	glm::quat defaultSpaceshipRot;
	float spaceshipMenuRotSpeed;

	CameraPtr camera;
	glm::vec3 defaultCameraPos;

	std::shared_ptr<float> lifeFracValue;
	std::shared_ptr<float> shieldFracValue;
	std::shared_ptr<int> pointsScore;
	std::shared_ptr<int> highScore;

	int prevScoreVal;
	float scoreUpdateAnimMaxTime;
	float scoreUpdateAnimCurrTime;

	float menuBeginTextAnimCurrTime;
	float menuBeginTextAnimMaxTime;
	float beginTextColorAnimDir;

	StatusBarRendererPtr shieldRenderer;
	StatusBarRendererPtr lifeRenderer;

	void updateSatusBars();
	void renderSpaceship();
	void renderInGameScreen();
	void renderEndGameScreen();
};

typedef std::shared_ptr<Gui> GuiPtr;

#pragma once

#include <GameResources/gameobjects/gameobject.h>
#include <fstream>
#include <array>

struct GameConfigData
{
	float mainEngineThrust;
	float maneuverEngineThrust;
	float spaceshipMass;
	float maneuverEngineForceArmLength;
	bool advancedFlightMode;

	float asteroidsSpawnFreq;
	float asteroidsSpawnAcceleration;
	float asteroidStandardHealth;
	float asteroidMinAngualrVelocity;
	float asteroidMaxAngularVelocity;

	float spaceshipHealth;
	float spaceshipShieldsHealth;
	float spaceshipShotDamage;
	float spaceshipFireRate;
	float spaceshipLaserShotSpeed;

	float forwardSpeed;
	float explosionDuration;

	float particlesCountFactor;

	bool showHitboxes;

	bool mouseInput;

	GameConfigData();
};

struct FileLineStringData
{
	std::string tag;
	std::string value;
};

class GameManager
{
public:
	enum ErrorCode { NO_ERROR, CONFIG_FILE_NOT_AVAILABLE, RESOURCE_FILE_NOT_AVAILABLE };
	enum GameState { INIT, MAIN_MENU, GAME, END_GAME, CRASH };

	GameManager();

	void intitializeGameData();
	void launchMainMenu();
	void launchMainGame();
	void launchEndGameScreen();

	void checkEndGame();

	GameConfigData getConfigData()const;
	ErrorCode getErrorCode()const;
	std::string getNotFoundResourceFilename()const;

	GameState getGameState()const;

	std::shared_ptr<bool> getEndGameFlag()const;
	std::shared_ptr<int> getHighScorePtr()const;
	void registerLastScore(int score);

private:
	ErrorCode errorCode;
	GameConfigData configData;
	std::string lastNotFoundResourceFile;
	GameState currGameState;

	std::shared_ptr<bool> endGameFlag;

	std::shared_ptr<int> highScore;

	const float eps = 0.0000001f;

	const std::vector<std::string> resources { "hitboxes/asteroidHitbox.hbx", "hitboxes/bodyHitbox.hbx", "hitboxes/cocpitHitbox.hbx",
		"hitboxes/hitbox1.hbx", "hitboxes/mainEngineHitbox.hbx", "hitboxes/wingHitbox.hbx", "models/asteroid_1.mgr", "models/spaceship.mgr",
		"models/textures/texture.png", "models/textures/asteroid_1_texture.png", "shaders/laserShader.frag", "shaders/laserShader.vert",
		"shaders/particleV2.vert", "shaders/particle.frag", "shaders/spaceshipShader.vert","shaders/spaceshipShader.frag",
		"shaders/spaceshipShaderTex.vert", "shaders/spaceshipShaderTex.frag", "shaders/standardHitbox.vert", "shaders/standardHitbox.frag",
		"shaders/statusBar.vert", "shaders/statusBar.frag", "shaders/texLaserShaders.vert", "shaders/texLaserShaders.frag",
		"sprites/asteroid_particle.png", "sprites/laser_beam_2.png", "sprites/smoke.png" };
		
	void checkExternalResources();

	void readConfigFile();
	void processConfigFile(std::fstream& file);
	std::string getCleanFullLine(std::fstream& file);
	bool checkFileLineData(const std::string& line);
	FileLineStringData getDataFromLine(const std::string& line);
	bool checkCorrectData(const std::string& val);
	void processLineData(const FileLineStringData& lineData);

	void loadMainEngineThrust(const FileLineStringData& lineData);
	void loadManeuverEngineThrust(const FileLineStringData& lineData);
	void loadSpaceshipMass(const FileLineStringData& lineData);
	void loadManeuverEngineForceArmLength(const FileLineStringData& lineData);
	void loadAdvancedFlightMode(const FileLineStringData& lineData);
	void loadAsteroidsSpawnFreq(const FileLineStringData& lineData);
	void loadAsteroidsSpawnAcceleration(const FileLineStringData& lineData);
	void loadAsteroidStandardHealth(const FileLineStringData& lineData);
	void loadAsteroidMinAngualrVelocity(const FileLineStringData& lineData);
	void loadAsteroidMaxAngularVelocity(const FileLineStringData& lineData);
	void loadSpaceshipHealth(const FileLineStringData& lineData);
	void loadSpaceshipShieldsHealth(const FileLineStringData& lineData);
	void loadSpaceshipShotDamage(const FileLineStringData& lineData);
	void loadSpaceshipFireRate(const FileLineStringData& lineData);
	void loadSpaceshipLaserShotSpeed(const FileLineStringData& lineData);
	void loadForwardSpeed(const FileLineStringData& lineData);
	void loadExplosionDuration(const FileLineStringData& lineData);
	void loadParticlesCountFactor(const FileLineStringData& lineData);
	void loadShowHitboxes(const FileLineStringData& lineData);
	void loadMousInput(const FileLineStringData& lineData);

	void readHighScore();
	void saveHighScoreToFile();
};

typedef std::shared_ptr<GameManager> GameManagerPtr;

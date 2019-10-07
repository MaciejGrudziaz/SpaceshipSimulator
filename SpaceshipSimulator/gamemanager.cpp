#include "gamemanager.h"

GameConfigData::GameConfigData()
	: mainEngineThrust(100000.0f)
	, maneuverEngineThrust(10000.0f)
	, spaceshipMass(1000.0f)
	, maneuverEngineForceArmLength(1.0f)
	, advancedFlightMode(false)
	, asteroidsSpawnFreq(0.5f)
	, asteroidsSpawnAcceleration(0.2f)
	, asteroidStandardHealth(100.0f)
	, asteroidMinAngualrVelocity(glm::radians(45.0f))
	, asteroidMaxAngularVelocity(glm::radians(90.0f))
	, spaceshipHealth(500.0f)
	, spaceshipShieldsHealth(500.0f)
	, spaceshipShotDamage(25.0f)
	, spaceshipFireRate(10.0f)
	, spaceshipLaserShotSpeed(20.0f)
	, forwardSpeed(5.0f)
	, explosionDuration(3.0f)
	, particlesCountFactor(1.0f)
	, showHitboxes(false)
{}

GameManager::GameManager()
	: endGameFlag(std::make_shared<bool>(false))
	, highScore(std::make_shared<int>(0))
	, errorCode(NO_ERROR)
{}

void GameManager::intitializeGameData()
{
	currGameState = INIT;

	readConfigFile();

	if (errorCode == NO_ERROR)
		checkExternalResources();

	readHighScore();
}

void GameManager::readConfigFile()
{
	std::fstream file("config.cfg");

	if (file.is_open())
	{
		processConfigFile(file);
		file.close();
	}
	else
	{
		errorCode = CONFIG_FILE_NOT_AVAILABLE;
		currGameState = CRASH;
	}
}

void GameManager::processConfigFile(std::fstream& file)
{
	while (!file.eof())
	{
		std::string line = getCleanFullLine(file);
		if (checkFileLineData(line))
		{
			FileLineStringData lineData = getDataFromLine(line);
			if (checkCorrectData(lineData.value))
			{
				processLineData(lineData);
			}
		}

	}
}

std::string GameManager::getCleanFullLine(std::fstream& file)
{
	std::array<char, 256> buffer;
	std::string rawLine;
	file.getline(buffer.data(), buffer.size());
	rawLine += buffer.data();

	while (file.fail() && !file.eof())
	{
		file.clear();
		file.getline(buffer.data(), buffer.size());
		rawLine += buffer.data();
	}

	size_t pos = rawLine.find_first_of('#');
	if (pos != std::string::npos)
		rawLine = rawLine.substr(0, pos);

	std::string cleanLine;
	for (int i = 0; i < rawLine.size(); ++i)
	{
		if (rawLine[i] != ' ' && rawLine[i] != '\t')
			cleanLine.push_back(rawLine[i]);
	}

	return cleanLine;
}

bool GameManager::checkFileLineData(const std::string& line)
{
	bool isCorrect = false;
	size_t pos = line.find_first_of('=');

	if (pos != std::string::npos)
	{
		pos = line.substr(pos + 1).find_first_of('=');
		if (pos == std::string::npos)
			isCorrect = true;
	}

	return isCorrect;
}

FileLineStringData GameManager::getDataFromLine(const std::string& line)
{
	FileLineStringData lineData;
	size_t pos = line.find_first_of('=');
	lineData.tag = line.substr(0, pos);
	lineData.value = line.substr(pos + 1);

	return lineData;
}

bool GameManager::checkCorrectData(const std::string& val)
{
	try {
		std::stof(val);
	}
	catch (const std::invalid_argument& e)
	{
		return false;
	}
	catch (const std::out_of_range& e)
	{
		return false;
	}

	bool isCorect = true;
	for (int i = 0; i < val.size(); ++i)
	{
		if (val[i] != '.' && val[i] != '-')
		{
			if (int(val[i]) < 48 || int(val[i]) > 57)
			{
				isCorect = false;
				break;
			}
		}
	}

	return isCorect;
}

void GameManager::processLineData(const FileLineStringData& lineData)
{
	if (lineData.tag == "mainEngineThrust")
		loadMainEngineThrust(lineData);
	if (lineData.tag == "maneuverEngineThrust")
		loadManeuverEngineThrust(lineData);
	if (lineData.tag == "spaceshipMass")
		loadSpaceshipMass(lineData);
	if (lineData.tag == "maneuverEngineForceArmLength")
		loadManeuverEngineForceArmLength(lineData);
	if (lineData.tag == "advancedFlightMode")
		loadAdvancedFlightMode(lineData);
	if (lineData.tag == "asteroidsSpawnFreq")
		loadAsteroidsSpawnFreq(lineData);
	if (lineData.tag == "asteroidsSpawnAcceleration")
		loadAsteroidsSpawnAcceleration(lineData);
	if (lineData.tag == "asteroidStandardHealth")
		loadAsteroidStandardHealth(lineData);
	if (lineData.tag == "asteroidMinAngualrVelocity")
		loadAsteroidMinAngualrVelocity(lineData);
	if (lineData.tag == "asteroidMaxAngularVelocity")
		loadAsteroidMaxAngularVelocity(lineData);
	if (lineData.tag == "spaceshipHealth")
		loadSpaceshipHealth(lineData);
	if (lineData.tag == "spaceshipShieldsHealth")
		loadSpaceshipShieldsHealth(lineData);
	if (lineData.tag == "spaceshipShotDamage")
		loadSpaceshipShotDamage(lineData);
	if (lineData.tag == "spaceshipFireRate")
		loadSpaceshipFireRate(lineData);
	if (lineData.tag == "spaceshipLaserShotSpeed")
		loadSpaceshipLaserShotSpeed(lineData);
	if (lineData.tag == "forwardSpeed")
		loadForwardSpeed(lineData);
	if (lineData.tag == "explosionDuration")
		loadExplosionDuration(lineData);
	if (lineData.tag == "particlesCountFactor")
		loadParticlesCountFactor(lineData);
	if (lineData.tag == "showHitboxes")
		loadShowHitboxes(lineData);
	if (lineData.tag == "mouseInput")
		loadMousInput(lineData);
}

void GameManager::loadMainEngineThrust(const FileLineStringData& lineData)
{
	configData.mainEngineThrust = std::stof(lineData.value);
	if (configData.mainEngineThrust < 0.0f) configData.mainEngineThrust = 0.0f;
}

void GameManager::loadManeuverEngineThrust(const FileLineStringData& lineData)
{
	configData.maneuverEngineThrust = std::stof(lineData.value);
	if (configData.maneuverEngineThrust < 0.0f) configData.maneuverEngineThrust = 0.0f;
}

void GameManager::loadSpaceshipMass(const FileLineStringData& lineData)
{
	configData.spaceshipMass = std::stof(lineData.value);
	if (configData.spaceshipMass <= 0.0f) configData.spaceshipMass = eps;
}

void GameManager::loadManeuverEngineForceArmLength(const FileLineStringData& lineData)
{
	configData.maneuverEngineForceArmLength = std::stof(lineData.value);
	if (configData.maneuverEngineForceArmLength < 0.0f) configData.maneuverEngineForceArmLength = 0.0f;
}

void GameManager::loadAdvancedFlightMode(const FileLineStringData& lineData)
{
	configData.advancedFlightMode = (std::stof(lineData.value) == 0.0f) ? false : true;
}

void GameManager::loadAsteroidsSpawnFreq(const FileLineStringData& lineData)
{
	configData.asteroidsSpawnFreq = std::stof(lineData.value);
	if (configData.asteroidsSpawnFreq <= 0.0f) configData.asteroidsSpawnFreq = eps;
}

void GameManager::loadAsteroidsSpawnAcceleration(const FileLineStringData& lineData)
{
	configData.asteroidsSpawnAcceleration = std::stof(lineData.value);
	if (configData.asteroidsSpawnAcceleration < 0.0f)  configData.asteroidsSpawnAcceleration = 0.0f;
}

void GameManager::loadAsteroidStandardHealth(const FileLineStringData& lineData)
{
	configData.asteroidStandardHealth = std::stof(lineData.value);
	if (configData.asteroidStandardHealth <= 0.0f) configData.asteroidStandardHealth = eps;
}

void GameManager::loadAsteroidMinAngualrVelocity(const FileLineStringData& lineData)
{
	configData.asteroidMinAngualrVelocity = std::stof(lineData.value);
	if (configData.asteroidMinAngualrVelocity < 0.0f) configData.asteroidMinAngualrVelocity = 0.0f;
}

void GameManager::loadAsteroidMaxAngularVelocity(const FileLineStringData& lineData)
{
	configData.asteroidMaxAngularVelocity = std::stof(lineData.value);
	if (configData.asteroidMaxAngularVelocity < 0.0f) configData.asteroidMaxAngularVelocity = 0.0f;
	if (configData.asteroidMaxAngularVelocity < configData.asteroidMinAngualrVelocity)
		configData.asteroidMaxAngularVelocity = configData.asteroidMinAngualrVelocity + eps;
}

void GameManager::loadSpaceshipHealth(const FileLineStringData& lineData)
{
	configData.spaceshipHealth = std::stof(lineData.value);
	if (configData.spaceshipHealth <= 0.0f) configData.spaceshipHealth = eps;
}

void GameManager::loadSpaceshipShieldsHealth(const FileLineStringData& lineData)
{
	configData.spaceshipShieldsHealth = std::stof(lineData.value);
	if (configData.spaceshipShieldsHealth < 0.0f) configData.spaceshipShieldsHealth = 0.0f;
}

void GameManager::loadSpaceshipShotDamage(const FileLineStringData& lineData)
{
	configData.spaceshipShotDamage = std::stof(lineData.value);
	if (configData.spaceshipShotDamage < 0.0f) configData.spaceshipShotDamage = 0.0f;
}

void GameManager::loadSpaceshipFireRate(const FileLineStringData& lineData)
{
	configData.spaceshipFireRate = std::stof(lineData.value);
	if (configData.spaceshipFireRate <= 0.0f) configData.spaceshipFireRate = eps;
}

void GameManager::loadSpaceshipLaserShotSpeed(const FileLineStringData& lineData)
{
	configData.spaceshipLaserShotSpeed = std::stof(lineData.value);
	if (configData.spaceshipLaserShotSpeed < 0.0f) configData.spaceshipLaserShotSpeed = 0.0f;
}

void GameManager::loadForwardSpeed(const FileLineStringData& lineData)
{
	configData.forwardSpeed = std::stof(lineData.value);
	if (configData.forwardSpeed < 0.0f) configData.forwardSpeed = 0.0f;
}

void GameManager::loadExplosionDuration(const FileLineStringData& lineData)
{
	configData.explosionDuration = std::stof(lineData.value);
	if (configData.explosionDuration < 0.0f) configData.explosionDuration = 0.0f;
}

void GameManager::loadParticlesCountFactor(const FileLineStringData& lineData)
{
	configData.particlesCountFactor = std::stof(lineData.value);
	if (configData.particlesCountFactor < 0.0f) configData.particlesCountFactor = 0.0f;
	if (configData.particlesCountFactor > 1.0f) configData.particlesCountFactor = 1.0f;
}

void GameManager::loadMousInput(const FileLineStringData& lineData)
{
	configData.mouseInput = (std::stof(lineData.value) == 0.0f) ? false : true;
}

void GameManager::loadShowHitboxes(const FileLineStringData& lineData)
{
	configData.showHitboxes = (std::stof(lineData.value) == 0.0f) ? false : true;
}

void GameManager::checkExternalResources()
{
	std::fstream file;
	for (auto fileName : resources)
	{
		file.open(fileName);
		if (!file.is_open())
		{
			errorCode = RESOURCE_FILE_NOT_AVAILABLE;
			currGameState = CRASH;
			lastNotFoundResourceFile = fileName;
			break;
		}
		else file.close();
	}
}

void GameManager::launchMainMenu()
{
	currGameState = MAIN_MENU;
}

void GameManager::launchMainGame()
{
	currGameState = GAME;
	*endGameFlag = false;
}

void GameManager::launchEndGameScreen()
{
	currGameState = END_GAME;
}

void GameManager::checkEndGame()
{
	if (currGameState == GAME)
	{
		if (*endGameFlag)
		{
			launchEndGameScreen();
		}
	}
}

GameConfigData GameManager::getConfigData()const
{
	return configData;
}

GameManager::ErrorCode GameManager::getErrorCode()const
{
	return errorCode;
}

std::string GameManager::getNotFoundResourceFilename()const
{
	return lastNotFoundResourceFile;
}

GameManager::GameState GameManager::getGameState()const
{
	return currGameState;
}

std::shared_ptr<bool> GameManager::getEndGameFlag()const
{
	return endGameFlag;
}

std::shared_ptr<int> GameManager::getHighScorePtr()const
{
	return highScore;
}

void GameManager::registerLastScore(int score)
{
	if (score > *highScore)
	{
		*highScore = score;
		saveHighScoreToFile();
	}
}

void GameManager::readHighScore()
{
	std::fstream file("hs.dat");

	if (file.is_open())
	{
		std::string line = getCleanFullLine(file);
		if (checkCorrectData(line))
		{
			int hs = std::stoi(line);
			if (hs < 0) hs = 0;
			*highScore = hs;
		}
		file.close();
	}
	else *highScore = 0;
}

void GameManager::saveHighScoreToFile()
{
	std::fstream file("hs.dat", std::ios::trunc | std::ios::out);
	file << *highScore;
	file.close();
}

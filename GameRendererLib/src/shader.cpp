#include <stdafx.h>

Shader::Shader(const std::string& name, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
	: errorCode(error::NO_ERROR)
	, vertexShader(-1)
	, fragmentShader(-1)
	, shaderProgram(-1)
	, name(name)
	, vertexShaderFilename(vertexShaderFilename)
	, fragmentShaderFilename(fragmentShaderFilename)
{
	loadVertexShader(vertexShaderFilename);
	loadFragmentShader(fragmentShaderFilename);
	linkShaderProgram();
}

void Shader::loadVertexShader(const std::string& vertexShaderFilename)
{
	if (!errorCode)
	{
		std::string shaderSource;
		shaderSource = loadShaderFromFile(vertexShaderFilename);

		if (!errorCode) 
		{
			vertexShader = glCreateShader(GL_VERTEX_SHADER);
			const GLchar* shaderSourceChar = shaderSource.c_str();
			glShaderSource(vertexShader, 1, &(shaderSourceChar), NULL);
			glCompileShader(vertexShader);

			int success;
			glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				errorCode = error::VERTEX_SHADER_ERROR;

				char infoLog[512];
				glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
				loadErrorToFile("ERROR::SHADER::VERTEX::COMPILATION_FAILED");
				loadErrorToFile(infoLog);
			}
		}
	}
}

std::string Shader::loadShaderFromFile(std::string filename)
{
	std::string fileStr;
	std::fstream file(filename, std::ios::in);

	if (!file.is_open()) {
		errorCode = error::FILE_NOT_AVAILABLE;
		loadErrorToFile("Shader file not found!");
		loadErrorToFile(filename.c_str());
	}
	else {
		char sign;
		do {
			sign = file.get();
			if (!file.eof())
				fileStr.push_back(sign);
		} while (!file.eof());
		file.close();

		fileStr = createGLSLHeader() + fileStr;
	}

	return fileStr;
}

std::string Shader::createGLSLHeader()
{
	const GLubyte* ver = glGetString(GL_VERSION);
	std::string verStr;
	char str[4];
	for (int i = 0; i<5; ++i)
	{
		if (i % 2 == 0)
			str[i / 2] = ver[i];
	}
	str[3] = '\0';

	verStr = "#version ";
	verStr += str;
	verStr += " core\n";
	
	return verStr;
}

void Shader::loadErrorToFile(const char* message)
{
	std::fstream file("LOGS/" + name + "_SHADER_ERRORLOG.txt", std::ios::out | std::ios::app);
	
	if (file.is_open())
	{
		auto time = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
		char buff[26];
		errno_t err = ctime_s(buff, 26, &time);

		if (!err) file << buff << " | ";
		file << message << std::endl;
		file.close();
	}
}

void Shader::loadFragmentShader(const std::string& fragmentShaderFilename)
{
	if (!errorCode)
	{
		std::string shaderSource;
		shaderSource = loadShaderFromFile(fragmentShaderFilename);

		if (!errorCode)
		{
			fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
			const GLchar* shaderSourceChar = shaderSource.c_str();
			glShaderSource(fragmentShader, 1, &(shaderSourceChar), NULL);
			glCompileShader(fragmentShader);

			int success;
			glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
			if (!success) {
				errorCode = error::FRAGMENT_SHADER_ERROR;

				char infoLog[512];
				glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
				loadErrorToFile("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED");
				loadErrorToFile(infoLog);
			}
		}
	}
}

void Shader::linkShaderProgram()
{
	shaderProgram = glCreateProgram();

	glAttachShader(shaderProgram, vertexShader);
	glAttachShader(shaderProgram, fragmentShader);

	GLenum err;
	err = glGetError();
	if (err != GL_NO_ERROR)
		errorCode = error::LINKING_ERROR;


	glLinkProgram(shaderProgram);
	err = glGetError();
	if (err != GL_NO_ERROR)
		errorCode = error::LINKING_ERROR;

	int success;
	glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
	if (!success) {
		errorCode = error::LINKING_ERROR;

		char infoLog[512];
		glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
		loadErrorToFile("ERROR::PROGRAM::LINKING_FAILED");
		loadErrorToFile(infoLog);
	}

	glDeleteShader(vertexShader);
	glDeleteShader(fragmentShader);

	err = glGetError();
}

int Shader::getProgram()const
{ 
	return shaderProgram; 
}

void Shader::setName(const std::string& name)
{
	this->name = name;
}

std::string Shader::getName()const
{
	return name;
}

std::string Shader::getVertexShaderFilename()const
{
	return vertexShaderFilename;
}

std::string Shader::getFragmentShaderFilename()const
{
	return fragmentShaderFilename;
}

Shader::error Shader::getErrorCode()const
{
	return errorCode;
}

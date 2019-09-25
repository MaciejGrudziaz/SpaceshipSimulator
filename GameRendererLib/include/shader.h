#pragma once

#include <gl/glew.h>
#include <string>
#include <fstream>
#include <assert.h>
#include <chrono>
#include <ctime>

class Shader
{
public:
	enum error { NO_ERROR, VERTEX_SHADER_ERROR, FRAGMENT_SHADER_ERROR, LINKING_ERROR, FILE_NOT_AVAILABLE, };

	Shader(const std::string& name, const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	void LoadVertexShader(const std::string& vertexShaderFilename_);
	void LoadFragmentShader(const std::string& fragmentShaderFilename_);

	void LinkShaderProgram();

	int getProgram()const;

	void setName(const std::string& name);
	std::string getName()const;

	std::string getVertexShaderFilename()const;
	std::string getFragmentShaderFilename()const;

	error getErrorCode()const;

private:
	int vertexShader;
	int fragmentShader;
	int shaderProgram;

	std::string name;
	std::string vertexShaderFilename;
	std::string fragmentShaderFilename;

	error errorCode;

	std::string loadShaderFromFile(std::string filename);
	std::string createGLSLHeader();
	void loadErrorToFile(const char* message);
};

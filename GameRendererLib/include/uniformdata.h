#pragma once

#include <string>
#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

struct UniformData
{
	std::string name;
	GLint location;

	UniformData(const std::string& name) 
		: name(name)
		, location(-1)
	{}

	virtual void loadLocation(GLuint shaderProgram)
	{
		location = glGetUniformLocation(shaderProgram, name.c_str());
	}
	virtual void update() = 0;
};

struct UniformDataMat4 : public UniformData
{
	UniformDataMat4(const std::string& name)
		: UniformData(name)
	{}

	std::shared_ptr<const glm::mat4> mat;

	void update() override
	{
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(*mat));
	}
};

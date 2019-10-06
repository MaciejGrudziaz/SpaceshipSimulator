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

	virtual bool isUsable()const
	{
		return true;
	}

	virtual void loadLocation(GLuint shaderProgram)
	{
		location = glGetUniformLocation(shaderProgram, name.c_str());
	}

	virtual void update() = 0;
};

struct NullUniformData : public UniformData
{
	NullUniformData()
		: UniformData("")
	{}

	bool isUsable()const override
	{
		return false;
	}

	void update()override
	{}
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

struct UniformDataVec2 : public UniformData
{
	UniformDataVec2(const std::string& name)
		: UniformData(name)
	{}

	std::shared_ptr<const glm::vec2> vec;

	void update()override
	{
		glUniform2fv(location, 1, glm::value_ptr(*vec));
	}
};

struct UniformDataVec3 : public UniformData
{
	UniformDataVec3(const std::string& name)
		: UniformData(name)
	{}

	std::shared_ptr<const glm::vec3> vec;

	void update()override
	{
		glUniform3fv(location, 1, glm::value_ptr(*vec));
	}
};

struct UniformDataVec4 : public UniformData
{
	UniformDataVec4(const std::string& name)
		: UniformData(name)
	{}

	std::shared_ptr<const glm::vec4> vec;

	void update()override
	{
		glUniform4fv(location, 1, glm::value_ptr(*vec));
	}
};

struct UniformDataFloat : public UniformData
{
	UniformDataFloat(const std::string& name)
		: UniformData(name)
	{}

	std::shared_ptr<const float> val;

	void update()override
	{
		glUniform1f(location, *val);
	}
};

struct UniformDataInt : public UniformData
{
	UniformDataInt(const std::string& name)
		: UniformData(name)
	{}

	std::shared_ptr<const int> val;

	void update()override
	{
		glUniform1i(location, *val);
	}
};

typedef std::shared_ptr<UniformData> UniformDataPtr;
typedef std::shared_ptr<UniformDataMat4> UniformDataMat4Ptr;
typedef std::shared_ptr<UniformDataVec2> UniformDataVec2Ptr;
typedef std::shared_ptr<UniformDataVec3> UniformDataVec3Ptr;
typedef std::shared_ptr<UniformDataVec4> UniformDataVec4Ptr;
typedef std::shared_ptr<UniformDataFloat> UniformDataFloatPtr;
typedef std::shared_ptr<UniformDataInt> UniformDataIntPtr;

typedef std::shared_ptr<const glm::mat4> ConstMat4Ptr;
typedef std::shared_ptr<const glm::vec2> ConstVec2Ptr;
typedef std::shared_ptr<const glm::vec3> ConstVec3Ptr;
typedef std::shared_ptr<const glm::vec4> ConstVec4Ptr;
typedef std::shared_ptr<const float> ConstFloatPtr;
typedef std::shared_ptr<const int> ConstIntPtr;

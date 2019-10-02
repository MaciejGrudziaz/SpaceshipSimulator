#pragma once

#include "shader.h"
#include "uniformdata.h"
#include <memory>
#include <glm/matrix.hpp>
#include <stack>
#include <list>
#include <vector>
#include <map>
#include <algorithm>

struct ShaderAttribute
{
	GLuint location;
	GLint size;
	GLvoid* offset;
};

class RenderObject
{
public:
	enum error { NO_ERROR, BUFFER_CREATE_ERROR, SHADER_CREATE_ERROR, NO_SHADER_AVAILABLE, UNIFORM_NOT_FOUND, BUFFER_DATA_ERROR, 
		VERTEX_ATTRIB_POINTER_NOT_VALID, TEXTURE_FILE_ERROR };

	RenderObject();
	RenderObject(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	void setActive(bool status);
	bool isActive()const;

	virtual void deepCopy(const RenderObject& object);

	void loadShader(std::shared_ptr<Shader> shader);
	void loadShader(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	void addUniform(std::shared_ptr<UniformData> uniform);
	void deleteUniform(const std::string& name);
	UniformDataPtr getUniform(const std::string& name);

	void addShaderAttribute(ShaderAttribute attribute);
	void loadBuffer(const std::vector<float>& verticesBuffer);

	virtual void init();
	virtual void process();
	virtual void invalidate();

	error getErrorCode();

protected:
	bool activeFlag;
	std::shared_ptr<Shader> shader;
	GLuint VAO;
	GLuint VBO;
	int bufferVerticesCount;

	typedef std::map<std::string, std::shared_ptr<UniformData> > UniformMap;
	UniformMap uniforms;

	typedef std::list<ShaderAttribute> ShaderAttrList;
	ShaderAttrList shaderAttributes;
	GLsizei shaderAttributesTotalSize;

	std::stack<error> errorCode;

	void initBuffersAndArrays();
	void loadUniformsLoc();

	void bindVertexArray();
	void updateUniforms();
	void drawArrays();
};

typedef std::shared_ptr<RenderObject> RenderObjectPtr;

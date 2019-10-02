#include "beamsrenderer.h"

BeamsRenderer::BeamsRenderer(std::vector<float>& buffer, int bufferSize)
	: buffer(buffer)
	, currBufferSize(bufferSize)
	, updateBufferFlag(false)
	, maxBufferSize(200 * 6)
{}

void BeamsRenderer::init()
{
	RenderObject::init();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, maxBufferSize * sizeof(float), NULL, GL_STREAM_DRAW);
}

void BeamsRenderer::process()
{
	static int test = 0;
	++test;
	if (shader->getErrorCode() == Shader::NO_ERROR && activeFlag)
	{
     	glUseProgram(shader->getProgram());
		glBindVertexArray(VAO);

		if (updateBufferFlag)
			updateBuffer();
			
		glBindBuffer(GL_ARRAY_BUFFER, VBO);

		updateUniforms();

		glDrawArrays(GL_LINES, 0, 2);

	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void BeamsRenderer::updateBuffer()
{
	currBufferSize = buffer.size();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, maxBufferSize * sizeof(float), NULL, GL_STREAM_DRAW);
	glBufferSubData(GL_ARRAY_BUFFER, 0, buffer.size() * sizeof(float), buffer.data());

	updateBufferFlag = false;
}

void BeamsRenderer::enableVertexAttribPointers()
{
	for (ShaderAttrList::value_type shaderAttrEntry : shaderAttributes)
	{
		glVertexAttribPointer(shaderAttrEntry.location, shaderAttrEntry.size, GL_FLOAT, GL_FALSE, shaderAttributesTotalSize * sizeof(float), shaderAttrEntry.offset);
		glEnableVertexAttribArray(shaderAttrEntry.location);
	}
}

void BeamsRenderer::setUpdateBufferFlag()
{
	updateBufferFlag = true;
}

void BeamsRenderer::loadShaders()
{
	RenderObject::loadShader(vertexShader, fragmentShader);
}
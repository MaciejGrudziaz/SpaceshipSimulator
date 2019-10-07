#include "laserrenderer.h"

LaserRenderer::LaserRenderer(std::vector<float>& buffer)
	: buffer(buffer)
	, currBufferSize(0)
	, updateBufferFlag(false)
	, maxBufferSize(500 * 6)
{}

void LaserRenderer::init()
{
	RenderObject::init();

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, maxBufferSize * sizeof(float), NULL, GL_STREAM_DRAW);
}

void LaserRenderer::process()
{
	static int test = 0;
	++test;
	if (shader->getErrorCode() == Shader::NO_ERROR && activeFlag)
	{
		glLineWidth(5.0f);

        glUseProgram(shader->getProgram());
		glBindVertexArray(VAO);

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		updateBuffer();
			
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);

		updateUniforms();

		glDrawArrays(GL_LINES, 0, currBufferSize/3);

		glDisableVertexAttribArray(0);

		glLineWidth(1.0f);
	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void LaserRenderer::updateBuffer()
{
	static int test = 0;
	++test;

	if (updateBufferFlag)
	{
		currBufferSize = buffer.size();

		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, maxBufferSize * sizeof(float), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, currBufferSize * sizeof(float), buffer.data());

		updateBufferFlag = false;
	}
}

void LaserRenderer::enableVertexAttribPointers()
{
	for (ShaderAttrList::value_type shaderAttrEntry : shaderAttributes)
	{
		glVertexAttribPointer(shaderAttrEntry.location, shaderAttrEntry.size, GL_FLOAT, GL_FALSE, shaderAttributesTotalSize, shaderAttrEntry.offset);
		glEnableVertexAttribArray(shaderAttrEntry.location);
	}
}

void LaserRenderer::setUpdateBufferFlag()
{
	updateBufferFlag = true;
}

void LaserRenderer::loadShaders()
{
	RenderObject::loadShader(vertexShader, fragmentShader);
}
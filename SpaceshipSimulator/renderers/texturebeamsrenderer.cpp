#include "texturebeamsrenderer.h"

TextureBeamsRenderer::TextureBeamsRenderer(std::vector<float>& posRotBuffer)
	: posRotBuffer(posRotBuffer)
	, maxBufferSize(vertexAttribCount * 100)
	, currBufferSize(0)
	, updateBufferFlag(false)
{}

void TextureBeamsRenderer::init()
{
	TextureRenderObject::init();

	glGenBuffers(1, &posRotBufferId);
}

void TextureBeamsRenderer::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR && activeFlag)
	{
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glUseProgram(shader->getProgram());
		glBindVertexArray(VAO);

		bindBuffers();
		updateBuffers();

		bindTexture();
		updateUniforms();

		glVertexAttribDivisor(0, 0);
		glVertexAttribDivisor(1, 1);
		glVertexAttribDivisor(2, 1);

		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, currBufferSize / (3+4));

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		glDisable(GL_BLEND);
	}
	else errorCode.push(NO_SHADER_AVAILABLE);
}

void TextureBeamsRenderer::bindBuffers()
{
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, posRotBufferId);
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
	//glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, (void*)(3 * sizeof(float)));
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, (3 + 4) * sizeof(float), (void*)0);
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, (3 + 4) * sizeof(float), (void*)(3*sizeof(float)));

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void TextureBeamsRenderer::updateBuffers()
{
	if (updateBufferFlag)
	{
		currBufferSize = posRotBuffer.size();

		if (currBufferSize > maxBufferSize)
			maxBufferSize = 2 * maxBufferSize;

		glBindBuffer(GL_ARRAY_BUFFER, posRotBufferId);
		glBufferData(GL_ARRAY_BUFFER, maxBufferSize * sizeof(float), NULL, GL_STREAM_DRAW);
		glBufferSubData(GL_ARRAY_BUFFER, 0, currBufferSize * sizeof(float), posRotBuffer.data());

		updateBufferFlag = false;
	}
}

void TextureBeamsRenderer::invalidate()
{
	TextureRenderObject::invalidate();

	glDeleteBuffers(1, &posRotBufferId);
}

void TextureBeamsRenderer::loadBuffers()
{
	static const GLfloat vertexBuffer[] = {
		-0.5f, -0.5f,  0.0f,
		0.5f, -0.5f,  0.0f,
		-0.5f,  0.5f,  0.0f,
		0.5f,  0.5f,  0.0f,
	};

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, posRotBufferId);
	glBufferData(GL_ARRAY_BUFFER, maxBufferSize * sizeof(float), NULL, GL_STREAM_DRAW);
}

void TextureBeamsRenderer::setUpdateBufferFlag()
{
	updateBufferFlag = true;
}

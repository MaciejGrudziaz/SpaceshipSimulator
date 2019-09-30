#include "hitboxrenderer.h"

HitboxRenderer::HitboxRenderer()
	: indicesBuffer(0)
	, indicesCount(0)
{}

void HitboxRenderer::deepCopy(const RenderObject& object)
{
	RenderObject::deepCopy(object);
	this->indicesBuffer = static_cast<const HitboxRenderer*>(&object)->indicesBuffer;
	this->indicesCount = static_cast<const HitboxRenderer*>(&object)->indicesCount;
}

void HitboxRenderer::init()
{
	RenderObject::init();
	glGenBuffers(1, &indicesBuffer);
}

void HitboxRenderer::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR && isActive)
	{
		glUseProgram(shader->getProgram());

		updateUniforms();

		glBindVertexArray(VAO);
		//glBindBuffer(GL_ARRAY_BUFFER, VBO);
		//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
		
		glDrawElementsBaseVertex(GL_LINES, indicesCount, GL_UNSIGNED_INT, 0, 0);
	}
}

void HitboxRenderer::loadBuffer(const std::vector<float>& buffer, const std::vector<unsigned>& indices)
{
	RenderObject::loadBuffer(buffer);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, indicesBuffer);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * indices.size(), indices.data(), GL_STATIC_DRAW);

	indicesCount = indices.size() * 2;
}

void HitboxRenderer::invalidate()
{
	RenderObject::invalidate();
	glDeleteBuffers(1, &indicesBuffer);
}

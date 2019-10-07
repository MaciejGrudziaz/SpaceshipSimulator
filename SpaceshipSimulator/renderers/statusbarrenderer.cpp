#include "statusbarrenderer.h"

StatusBarRenderer::StatusBarRenderer()
{
	colorMax = std::make_shared<glm::vec3>();
	colorMin = std::make_shared<glm::vec3>();
	frac = std::make_shared<float>(0.0f);
	posMin = std::make_shared<glm::vec2>();
	posMax = std::make_shared<glm::vec2>();
}

void StatusBarRenderer::initialize(const std::string& vertexShaderFilenam, const std::string& fragmenShaderFilename)
{
	loadShader(vertexShaderFilenam, fragmenShaderFilename);
	initUniforms();

	glUseProgram(shader->getProgram());
	loadUniformsLoc();
}

void StatusBarRenderer::initUniforms()
{
	UniformDataVec3Ptr colorMaxUniform = std::make_shared<UniformDataVec3>("maxColor");
	UniformDataVec3Ptr colorMinUniform = std::make_shared<UniformDataVec3>("minColor");
	UniformDataVec2Ptr minPosUniform = std::make_shared<UniformDataVec2>("minPos");
	UniformDataVec2Ptr maxPosUniform = std::make_shared<UniformDataVec2>("maxPos");
	UniformDataFloatPtr fracUniform = std::make_shared<UniformDataFloat>("frac");

	colorMaxUniform->vec = colorMax;
	colorMinUniform->vec = colorMin;
	minPosUniform->vec = posMin;
	maxPosUniform->vec = posMax;
	fracUniform->val = frac;

	addUniform(colorMaxUniform);
	addUniform(colorMinUniform);
	addUniform(minPosUniform);
	addUniform(maxPosUniform);
	addUniform(fracUniform);
}

void StatusBarRenderer::process()
{
	if (shader->getErrorCode() == Shader::NO_ERROR && isActive())
	{
		glUseProgram(shader->getProgram());

		updateUniforms();

		glDrawArrays(GL_TRIANGLE_STRIP, 0, 5);
	}
}

void StatusBarRenderer::setMaxColor(const glm::vec3& colorMax)
{
	*(this->colorMax) = colorMax;
}

void StatusBarRenderer::setMinColor(const glm::vec3& colorMin)
{
	*(this->colorMin) = colorMin;
}

void StatusBarRenderer::setFrac(float val)
{
	*(this->frac) = val;
}

void StatusBarRenderer::setMinPos(const glm::vec2& pos)
{
	*(this->posMin) = pos;
}

void StatusBarRenderer::setMaxPos(const glm::vec2& pos)
{
	*(this->posMax) = pos;
}

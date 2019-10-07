#pragma once

#include <GameRenderer/renderobject.h>
#include <glm/vec3.hpp>

class StatusBarRenderer: public RenderObject
{
public:
	StatusBarRenderer();

	void initialize(const std::string& vertexShaderFilenam, const std::string& fragmenShaderFilename);

	void process()override;

	void setMaxColor(const glm::vec3& colorMax);
	void setMinColor(const glm::vec3& colorMin);
	void setFrac(float val);
	void setMinPos(const glm::vec2& pos);
	void setMaxPos(const glm::vec2& pos);

private:
	std::shared_ptr<glm::vec3> colorMax;
	std::shared_ptr<glm::vec3> colorMin;
	std::shared_ptr<float> frac;
	std::shared_ptr<glm::vec2> posMin;
	std::shared_ptr<glm::vec2> posMax;

	void initUniforms();
};

typedef std::shared_ptr<StatusBarRenderer> StatusBarRendererPtr;

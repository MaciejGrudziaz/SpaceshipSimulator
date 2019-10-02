#pragma once

#include <GameRenderer/renderobject.h>

class BeamsRenderer : public RenderObject
{
public:
	BeamsRenderer(std::vector<float>& buffer, int bufferSize = 600);

	void init()override;
	void process()override;

	void setUpdateBufferFlag();
	void loadShaders();

private:
	const std::string vertexShader = "shaders/laserShader.vert";
	const std::string fragmentShader = "shaders/laserShader.frag";

	std::vector<float>& buffer;
	int currBufferSize;
	int maxBufferSize;
	bool updateBufferFlag;

	void updateBuffer();

	void enableVertexAttribPointers();
};

typedef std::shared_ptr<BeamsRenderer> BeamsRendererPtr;

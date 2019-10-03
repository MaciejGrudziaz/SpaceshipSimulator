#pragma once

#include <GameRenderer/texturerenderobject.h>

class TextureBeamsRenderer : public TextureRenderObject
{
public:
	TextureBeamsRenderer(std::vector<float>& posRotBuffer, int maxBufferSize = (3+4) * 10);

	void init()override;
	void process()override;
	void invalidate()override;

	void loadBuffers();
	void setUpdateBufferFlag();

private:
	const std::string vertexShader = "shaders/laserShader.vert";
	const std::string fragmentShader = "shaders/laserShader.frag";

	GLuint posRotBufferId;

	std::vector<float>& posRotBuffer;
	int maxBufferSize;
	int currBufferSize;
	bool updateBufferFlag;

	void bindBuffers();
	void updateBuffers();
};

typedef std::shared_ptr<TextureBeamsRenderer> TextureBeamsRendererPtr;

#pragma once

#include <GameRenderer/texturerenderobject.h>

class TextureBeamsRenderer : public TextureRenderObject
{
public:
	static const int vertexAttribCount = 7;

	TextureBeamsRenderer(std::vector<float>& posRotBuffer);

	void init()override;
	void process()override;
	void invalidate()override;

	void loadBuffers();
	void setUpdateBufferFlag();

private:
	GLuint posRotBufferId;

	std::vector<float>& posRotBuffer;
	int maxBufferSize;
	int currBufferSize;
	bool updateBufferFlag;

	void bindBuffers();
	void updateBuffers();
};

typedef std::shared_ptr<TextureBeamsRenderer> TextureBeamsRendererPtr;

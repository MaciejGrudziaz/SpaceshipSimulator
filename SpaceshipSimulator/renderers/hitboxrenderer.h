#pragma once

#include <GameRenderer/renderobject.h>

class HitboxRenderer: public RenderObject
{
public:
	HitboxRenderer();

	virtual void deepCopy(const RenderObject& object);

	void init()override;
	void process()override;
	void invalidate()override;

	void loadBuffer(const std::vector<float>& buffer, const std::vector<unsigned>& indices);

private:
	GLuint indicesBuffer;
	int indicesCount;
};

typedef std::shared_ptr<HitboxRenderer> HitboxRendererPtr;

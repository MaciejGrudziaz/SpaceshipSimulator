#pragma once

#include <GameRenderer/texturerenderobject.h>

class MultipleAsteroidsRenderer: public RenderObject
{
public:

	void init()override;
	void process()override;
	void invalidate()override;

	void registerRenderer(RenderObjectPtr renderer);

private:
	std::vector<RenderObjectPtr> asteroidsRenderers;

};

typedef std::shared_ptr<MultipleAsteroidsRenderer> MultipleAsteroidsRendererPtr;

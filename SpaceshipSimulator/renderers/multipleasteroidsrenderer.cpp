#include "multipleasteroidsrenderer.h"

void MultipleAsteroidsRenderer::init()
{
	for (auto asteroid : asteroidsRenderers)
	{
		asteroid->init();
	}
}
void MultipleAsteroidsRenderer::process()
{
	for (auto asteroid : asteroidsRenderers)
	{
		asteroid->process();
	}
}

void MultipleAsteroidsRenderer::invalidate()
{
	for (auto asteroid : asteroidsRenderers)
	{
		asteroid->invalidate();
	}
}

void MultipleAsteroidsRenderer::registerRenderer(RenderObjectPtr renderer)
{
	asteroidsRenderers.push_back(renderer);
}

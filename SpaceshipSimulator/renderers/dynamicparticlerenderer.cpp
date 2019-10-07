#include "dynamicparticlerenderer.h"

#ifdef OLD_VERSION

void DynamicParticleRenderer::process()
{
	std::for_each(renderers.begin(), renderers.end(), [](auto renderer) {
		renderer->process();
	});

	clearVector();
}

void DynamicParticleRenderer::clearVector()
{
	static int counter = 0;
	++counter;

	if (counter == 100)
	{
		for (std::vector<ParticleRendererPtr>::iterator rendererIt = renderers.begin(); rendererIt != renderers.end(); ++rendererIt)
		{
			if (!(*rendererIt)->isActive())
			{
				(*rendererIt)->invalidate();
				renderers.erase(rendererIt);
			}
		}

		counter = 0;
	}
}

void DynamicParticleRenderer::invalidate()
{
	std::for_each(renderers.begin(), renderers.end(), [](auto renderer) {
		renderer->invalidate();
	});
}

void DynamicParticleRenderer::registerParticleRenderer(ParticleRendererPtr renderer)
{
	renderers.push_back(renderer);
}

#endif

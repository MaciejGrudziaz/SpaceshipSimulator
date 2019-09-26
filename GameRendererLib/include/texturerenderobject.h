#pragma once

#include "renderobject.h"

class TextureRenderObject: public RenderObject
{
public:
	TextureRenderObject();
	TextureRenderObject(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	void loadTexture(const std::string& filename);

	virtual void process() override;
	virtual void invalidate() override;

protected:
	GLuint texture;

	bool checkTextureFile(const std::string& filename);

	void bindTexture();
};

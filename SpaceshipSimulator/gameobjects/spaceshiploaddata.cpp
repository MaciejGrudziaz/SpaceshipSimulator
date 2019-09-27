#include "spaceship.h"

SpaceshipLoadData::SpaceshipLoadData(Spaceship& spaceship, const SpaceshipData& data, const SpaceshipUniforms& uniforms)
	: spaceship(spaceship)
	, loadFiles(data)
	, uniforms(uniforms)
{}

void SpaceshipLoadData::loadModel()
{
	MgrImportFile modelImport;
	modelImport.import(loadFiles.modelFilename);

	if (!modelImport.getErrorFlag())
	{
		if (modelImport.getModel() && modelImport.getModel()->getObjectsCount() > 0)
		{
			modelImport.getModel()->getObject(0)->name = "spaceship";
			spaceship.addModel(modelImport.getModel()->getObject(0));
			spaceship.mainHitbox = modelImport.getModel()->getMainHitbox(0);
		}
	}
	else spaceship.errorCode.push(Spaceship::FILE_DATA_ERROR);
}

void SpaceshipLoadData::initUniforms()
{
	UniformDataMat4Ptr modelUniform = std::make_shared<UniformDataMat4>("model");
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");

	modelUniform->mat = spaceship.modelTransform;
	viewUniform->mat = uniforms.view;
	projectionUniform->mat = uniforms.projection;

	spaceship.renderer->addUniform(modelUniform);
	spaceship.renderer->addUniform(viewUniform);
	spaceship.renderer->addUniform(projectionUniform);
}

CleanSpaceshipLoadData::CleanSpaceshipLoadData(Spaceship& spaceship, const SpaceshipData& data, const SpaceshipUniforms& uniforms)
	: SpaceshipLoadData(spaceship, data, uniforms)
{}

void CleanSpaceshipLoadData::load()
{
	loadModel();
	loadShaders();
	initUniforms();
	initAttribPointers();
	spaceship.renderer->init();
	loadBufferData();
}

void CleanSpaceshipLoadData::loadShaders()
{
	spaceship.renderer->loadShader(loadFiles.vertexShaderFilename, loadFiles.fragmentShaderFilename);
}

void CleanSpaceshipLoadData::initAttribPointers()
{
	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	spaceship.renderer->addShaderAttribute(posAttrib);
}

void CleanSpaceshipLoadData::loadBufferData()
{
	std::vector<float> buffer;

	BasicObjectPtr model = spaceship.getModel("spaceship");

	if (model->isUsable())
	{
		std::for_each(model->faces.begin(), model->faces.end(), [&model, &buffer](auto face) {
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
					buffer.push_back(model->controlPoints[face.indices[i]].coords[j]);
			}
		});
	}

	spaceship.renderer->loadBuffer(buffer);
}

TextureSpaceshipLoadData::TextureSpaceshipLoadData(Spaceship& spaceship, const SpaceshipData& data, const SpaceshipUniforms& uniforms)
	: SpaceshipLoadData(spaceship, data, uniforms)
{}

void TextureSpaceshipLoadData::load()
{
	loadModel();
	loadShaders();
	initUniforms();
	initAttribPointers();
	spaceship.renderer->init();
	loadBufferData();
	loadTexture();
}

void TextureSpaceshipLoadData::loadShaders()
{
	size_t pos = loadFiles.vertexShaderFilename.find_last_of('.');
	loadFiles.vertexShaderFilename = loadFiles.vertexShaderFilename.substr(0, pos) + "Tex.vert";

	pos = loadFiles.fragmentShaderFilename.find_last_of('.');
	loadFiles.fragmentShaderFilename = loadFiles.fragmentShaderFilename.substr(0, pos) + "Tex.frag";

	spaceship.renderer->loadShader(loadFiles.vertexShaderFilename, loadFiles.fragmentShaderFilename);
}

void TextureSpaceshipLoadData::initAttribPointers()
{
	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	spaceship.renderer->addShaderAttribute(posAttrib);

	ShaderAttribute texAttrib;
	texAttrib.location = 1;
	texAttrib.size = 2;
	texAttrib.offset = (void*)(3 * sizeof(float));

	spaceship.renderer->addShaderAttribute(texAttrib);
}

void TextureSpaceshipLoadData::loadBufferData()
{
	std::vector<float> buffer;

	BasicObjectPtr model = spaceship.getModel("spaceship");

	if (model->isUsable())
	{
		std::for_each(model->faces.begin(), model->faces.end(), [&model, &buffer](auto face) {
			for (int i = 0; i < 3; ++i)
			{
				for (int j = 0; j < 3; ++j)
					buffer.push_back(model->controlPoints[face.indices[i]].coords[j]);
				buffer.push_back(face.uv[i].x);
				buffer.push_back(face.uv[i].y);
			}
		});
	}

	spaceship.renderer->loadBuffer(buffer);
}

void TextureSpaceshipLoadData::loadTexture()
{
	(static_cast<TextureRenderObject*>(spaceship.renderer.get()))->loadTexture(loadFiles.textureFilename);
}

#include "spaceship.h"

Spaceship::Spaceship()
	: renderer(std::make_shared<RenderObject>())
	, texRenderer(std::make_shared<TextureRenderObject>())
	, mainHitbox(std::make_shared<Hitbox>())
	, modelTransform(std::make_shared<glm::mat4>(1.0f))
{}

void Spaceship::load(const SpaceshipData& data, const SpaceshipUniforms& uniforms)
{
	std::unique_ptr<SpaceshipLoadData> loadSpaceship;
	std::fstream file(data.textureFilename);
	if (file.is_open())
	{
		file.close();
		renderer = std::make_shared<TextureRenderObject>();
		loadSpaceship = std::make_unique<TextureSpaceshipLoadData>(*this, data, uniforms);
	}
	else {
		renderer = std::make_shared<RenderObject>();
		loadSpaceship = std::make_unique<CleanSpaceshipLoadData>(*this, data, uniforms);
	}

	loadSpaceship->load();
}

void Spaceship::load(const std::string& filename)
{
	MgrImportFile modelImport;
	modelImport.import(filename);

	if (!modelImport.getErrorFlag())
	{
		if (modelImport.getModel() && modelImport.getModel()->getObjectsCount() > 0)
		{
			modelImport.getModel()->getObject(0)->name = "spaceship";
			addModel(modelImport.getModel()->getObject(0));
			mainHitbox = modelImport.getModel()->getMainHitbox(0);
		}
	}
	else errorCode.push(FILE_DATA_ERROR);
}

void Spaceship::loadShaders(const std::string& vertexShader, const std::string& fragmentShader)
{
	renderer->loadShader(vertexShader, fragmentShader);
}

void Spaceship::loadTextureShaders(const std::string& vertexShader, const std::string& fragmentShader)
{
	texRenderer->loadShader(vertexShader, fragmentShader);
}

void Spaceship::initRenderer(ConstMat4Ptr view, ConstMat4Ptr projection)
{
	UniformDataMat4Ptr modelUniform = std::make_shared<UniformDataMat4>("model");
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");

	modelUniform->mat = modelTransform;
	viewUniform->mat = view;
	projectionUniform->mat = projection;

	renderer->addUniform(modelUniform);
	renderer->addUniform(viewUniform);
	renderer->addUniform(projectionUniform);

	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	renderer->addShaderAttribute(posAttrib);

	renderer->init();
}

void Spaceship::initTextureRenderer(ConstMat4Ptr view, ConstMat4Ptr projection)
{
	UniformDataMat4Ptr modelUniform = std::make_shared<UniformDataMat4>("model");
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");

	modelUniform->mat = modelTransform;
	viewUniform->mat = view;
	projectionUniform->mat = projection;

	texRenderer->addUniform(modelUniform);
	texRenderer->addUniform(viewUniform);
	texRenderer->addUniform(projectionUniform);

	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	texRenderer->addShaderAttribute(posAttrib);

	ShaderAttribute texAttrib;
	texAttrib.location = 1;
	texAttrib.size = 2;
	texAttrib.offset = (void*)(3 * sizeof(float));

	texRenderer->addShaderAttribute(texAttrib);

	texRenderer->init();
}

void Spaceship::loadBufferData()
{
	std::vector<float> buffer;

	BasicObjectPtr model = getModel("spaceship");

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

	renderer->loadBuffer(buffer);
}

void Spaceship::loadTextureBufferData()
{
	std::vector<float> buffer;

	BasicObjectPtr model = getModel("spaceship");

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

	texRenderer->loadBuffer(buffer);
}

void Spaceship::loadTexture(const std::string& filename)
{
	texRenderer->loadTexture(filename);
}

std::shared_ptr<RenderObject> Spaceship::getRenderer()const
{
	return renderer;
}

std::shared_ptr<TextureRenderObject> Spaceship::getTextureRenderer()const
{
	return texRenderer;
}

Spaceship::error Spaceship::getErrorCode()
{
	error code;
	if (!errorCode.empty())
	{
		code = errorCode.top();
		errorCode.pop();
	}
	else code = NO_ERROR;

	return code;
}

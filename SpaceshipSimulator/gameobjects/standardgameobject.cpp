#include "standardgameobject.h"

StandardGameObject::StandardGameObject()
	: mainHitbox(std::make_shared<Hitbox>())
	, modelTransform(std::make_shared<glm::mat4>(1.0f))
	, textureAvailable(false)
{
	hitColor = std::make_shared<glm::vec3>(1.0f, 0.4f, 0.4f);
	maxHitTime = std::make_shared<float>(0.5f);
	currHitTime = std::make_shared<float>(*maxHitTime);
}

void StandardGameObject::deepCopy(const StandardGameObject& object)
{
	GameObject::deepCopy(object);

	this->textureAvailable = object.textureAvailable;

	if (this->textureAvailable)
		this->renderer = std::make_shared<TextureRenderObject>();
	else this->renderer = std::make_shared<RenderObject>();

	this->renderer->deepCopy(*object.renderer);

	this->mainHitbox = std::make_shared<Hitbox>();
	this->mainHitbox->deepCopy(*object.mainHitbox);

	this->modelTransform = std::make_shared<glm::mat4>(1.0f);
	this->hitColor = std::make_shared<glm::vec3>((*object.hitColor));
	this->currHitTime = std::make_shared<float>((*object.currHitTime));
	this->maxHitTime = std::make_shared<float>((*object.maxHitTime));

	copyModelUniform();
	copyHitColorUniforms();
}

void StandardGameObject::copyModelUniform()
{
	UniformDataPtr sourceModelUniform = this->renderer->getUniform("model");
	int location = sourceModelUniform->location;
	this->renderer->deleteUniform("model");
	UniformDataMat4Ptr modelUniform = std::make_shared<UniformDataMat4>("model");
	modelUniform->mat = this->modelTransform;
	modelUniform->location = location;
	this->renderer->addUniform(modelUniform);
}

void StandardGameObject::copyHitColorUniforms()
{
	UniformDataPtr sourceHitColorUniform = this->renderer->getUniform("hitColor");
	int location = sourceHitColorUniform->location;
	this->renderer->deleteUniform("hitColor");
	UniformDataVec3Ptr hitColorUniform = std::make_shared<UniformDataVec3>("hitColor");
	hitColorUniform->vec = this->hitColor;
	hitColorUniform->location = location;
	this->renderer->addUniform(hitColorUniform);

	UniformDataPtr sourceCurrHitColorUniform = this->renderer->getUniform("hitColorRunTime");
	location = sourceCurrHitColorUniform->location;
	this->renderer->deleteUniform("hitColorRunTime");
	UniformDataFloatPtr currHitColorUniform = std::make_shared<UniformDataFloat>("hitColorRunTime");
	currHitColorUniform->val = this->currHitTime;
	currHitColorUniform->location = location;
	this->renderer->addUniform(currHitColorUniform);

	UniformDataPtr sourceMaxHitColorUniform = this->renderer->getUniform("hitColorMaxTime");
	location = sourceMaxHitColorUniform->location;
	this->renderer->deleteUniform("hitColorMaxTime");
	UniformDataFloatPtr maxHitColorUniform = std::make_shared<UniformDataFloat>("hitColorMaxTime");
	maxHitColorUniform->val = this->maxHitTime;
	maxHitColorUniform->location = location;
	this->renderer->addUniform(maxHitColorUniform);
}

void StandardGameObject::load(const ModelData& data, const ModelExternalUniforms& uniforms)
{
	checkTextureFile(data.textureFilename);

	initializeRenderer();
	loadModel(data.modelFilename);
	loadShaders(data.vertexShaderFilename, data.fragmentShaderFilename);
	initUniforms(uniforms);
	initAttribPointers();
	renderer->init();
	loadBufferData();
	loadTexture(data.textureFilename);
}

void StandardGameObject::checkTextureFile(const std::string& texFilename)
{
	std::fstream file(texFilename);

	if (file.is_open())
	{
		textureAvailable = true;
		file.close();
	}
	else textureAvailable = false;
}

void StandardGameObject::initializeRenderer()
{
	if (textureAvailable)
		renderer = std::make_shared<TextureRenderObject>();
	else renderer = std::make_shared<RenderObject>();
}

void StandardGameObject::loadModel(const std::string& modelFilename)
{
	MgrImportFile modelImport;
	modelImport.import(modelFilename);

	if (!modelImport.getErrorFlag())
	{
		if (modelImport.getModel() && modelImport.getModel()->getObjectsCount() > 0)
		{
			modelImport.getModel()->getObject(0)->name = "spaceship";
			addModel(modelImport.getModel()->getObject(0));
			mainHitbox = modelImport.getModel()->getMainHitbox(0);
		}
	}
	else errorCode.push(StandardGameObject::MODEL_FILE_ERROR);
}

void StandardGameObject::loadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	if (textureAvailable)
		loadTextureShaders(vertexShaderFilename, fragmentShaderFilename);
	else loadStandardShaders(vertexShaderFilename, fragmentShaderFilename);
}

void StandardGameObject::loadStandardShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	renderer->loadShader(vertexShaderFilename, fragmentShaderFilename);
}

void StandardGameObject::loadTextureShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename)
{
	size_t pos = vertexShaderFilename.find_last_of('.');
	std::string textureVertexShaderFilename = vertexShaderFilename.substr(0, pos) + "Tex.vert";

	pos = fragmentShaderFilename.find_last_of('.');
	std::string textureFragmentShaderFilename = fragmentShaderFilename.substr(0, pos) + "Tex.frag";

	renderer->loadShader(textureVertexShaderFilename, textureFragmentShaderFilename);
}

void StandardGameObject::initUniforms(const ModelExternalUniforms& uniforms)
{
	UniformDataMat4Ptr modelUniform = std::make_shared<UniformDataMat4>("model");
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");
	UniformDataVec3Ptr hitColorUniform = std::make_shared<UniformDataVec3>("hitColor");
	UniformDataFloatPtr currHitColorTimeUniform = std::make_shared<UniformDataFloat>("hitColorRunTime");
	UniformDataFloatPtr maxHitColorTimeUniform = std::make_shared<UniformDataFloat>("hitColorMaxTime");

	modelUniform->mat = modelTransform;
	viewUniform->mat = uniforms.view;
	projectionUniform->mat = uniforms.projection;
	hitColorUniform->vec = hitColor;
	currHitColorTimeUniform->val = currHitTime;
	maxHitColorTimeUniform->val = maxHitTime;

	renderer->addUniform(modelUniform);
	renderer->addUniform(viewUniform);
	renderer->addUniform(projectionUniform);
	renderer->addUniform(hitColorUniform);
	renderer->addUniform(currHitColorTimeUniform);
	renderer->addUniform(maxHitColorTimeUniform);
}

void StandardGameObject::initAttribPointers()
{
	if (textureAvailable)
		initTextureAttribPointers();
	else initStandardAttribPointers();
}

void StandardGameObject::initStandardAttribPointers()
{
	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	renderer->addShaderAttribute(posAttrib);
}

void StandardGameObject::initTextureAttribPointers()
{
	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	renderer->addShaderAttribute(posAttrib);

	ShaderAttribute texAttrib;
	texAttrib.location = 1;
	texAttrib.size = 2;
	texAttrib.offset = (void*)(3 * sizeof(float));

	renderer->addShaderAttribute(texAttrib);
}

void StandardGameObject::loadBufferData()
{
	if (textureAvailable)
		loadTextureBufferData();
	else loadStandardBufferData();
}

void StandardGameObject::loadStandardBufferData()
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

void StandardGameObject::loadTextureBufferData()
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

	renderer->loadBuffer(buffer);
}

void StandardGameObject::loadTexture(const std::string& textureFilename)
{
	if (textureAvailable)
	{
		(static_cast<TextureRenderObject*>(renderer.get()))->loadTexture(textureFilename);
	}
}

void StandardGameObject::process()
{
	GameObject::process();

	*modelTransform = parentTransform * getTransform().getTransformMat();

	if ((*currHitTime) < (*maxHitTime))
		(*currHitTime) += Time::deltaTime;
}

void StandardGameObject::setActive(bool val)
{
	GameObject::setActive(val);

	renderer->setActive(val);
}

void StandardGameObject::dealDamage(float val)
{
	*currHitTime = 0.0f;
}

std::shared_ptr<RenderObject> StandardGameObject::getRenderer()const
{
	return renderer;
}

HitboxPtr StandardGameObject::getMainHitbox()const
{
	return mainHitbox;
;
}

StandardGameObject::error StandardGameObject::getErrorCode()
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

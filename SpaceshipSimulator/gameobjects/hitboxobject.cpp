#include "hitboxobject.h"

HitboxObject::HitboxObject()
	: renderer(std::make_shared<HitboxRenderer>())
	, hitbox(std::make_shared<Hitbox>())
	, modelTransform(std::make_shared<glm::mat4>(1.0f))
	, hitboxColor(std::make_shared<glm::vec3>(1.0f, 0.0f, 0.0f))
	, basicCollisionRadius(0.0f)
	, actualCollisionRadius(0.0f)
{}

void HitboxObject::load(const HitboxObjectData& filesData, const ModelExternalUniforms& uniforms)
{
	loadHitboxFromfile(filesData.hitboxFilename);
	initializeData(filesData, uniforms);
}

void HitboxObject::loadHitboxFromfile(const std::string& hitboxFilename)
{
	if (isHitboxFileCorrect(hitboxFilename))
	{
		std::fstream file(hitboxFilename);
		const int lineSize = 50;
		char delimCharacter = '\n';
		char buffer[lineSize];

		std::vector<glm::vec3> controlPt;
		std::vector<unsigned> indices;
		std::vector<glm::vec3> normals;
		while (!file.eof())
		{
			file.getline(buffer, lineSize, delimCharacter);
			std::string line(buffer);
			if (line == "vertices")
			{
				loadControlPoints(file, controlPt, lineSize, delimCharacter);
				loadIndices(file, indices, lineSize, delimCharacter);
				loadNormals(file, normals, lineSize, delimCharacter);
			}
		}

		file.close();

		for (int i = 0; i < 8; ++i)
		{
			hitbox->basicVertices[i] = controlPt[i];
		}
		for (int i = 0; i < 6; ++i)
		{
			hitbox->basicNormals[i] = normals[i];
		}
		hitbox->indices = indices;
	}
}

bool HitboxObject::isHitboxFileCorrect(const std::string& hitboxFilename)
{
	size_t pos = hitboxFilename.find_last_of('.');
	std::string extension = hitboxFilename.substr(pos + 1);
	bool isCorrect;

	if (extension == "hbx")
	{
		std::fstream file(hitboxFilename);
		if (file.is_open())
		{
			file.close();
			isCorrect = true;
		}
		else isCorrect = false;
	}
	else isCorrect = false;

	return isCorrect;
}

void HitboxObject::loadControlPoints(std::fstream& file, std::vector<glm::vec3>& controlPt, const int lineSize, char delimCharacter)
{
	std::unique_ptr<char> buffer(new char[lineSize]);
	std::string line;
	while (line != "indices" && !file.eof())
	{
		glm::vec3 vec;
		file.getline(buffer.get(), lineSize, delimCharacter);
		line = buffer.get();

		if (line != "indices" && line != "")
		{
			for (int i = 0; i < 3; ++i)
			{
				size_t pos = line.find_first_of(' ');
				std::string num = line.substr(0, pos);
				line = line.substr(pos + 1);

				vec[i] = std::stof(num);
			}
			controlPt.push_back(vec);
		}
	}
}

void HitboxObject::loadIndices(std::fstream& file, std::vector<unsigned>& indices, const int lineSize, char delimCharacter)
{
	std::unique_ptr<char> buffer(new char[lineSize]);
	std::string line;
	while (line!="normals" && !file.eof())
	{
		glm::vec3 vec;
		file.getline(buffer.get(), lineSize, delimCharacter);
		line = buffer.get();

		if (line != "" && line !="normals")
		{
			size_t pos = line.find_first_of(' ');
			std::string num = line.substr(0, pos);
			line = line.substr(pos + 1);

			indices.push_back(std::stoi(num));
			indices.push_back(std::stoi(line));
		}
	}
}

void HitboxObject::loadNormals(std::fstream& file, std::vector<glm::vec3>& normals, const int lineSize, char delimCharacter)
{
	std::unique_ptr<char> buffer(new char[lineSize]);
	std::string line;
	while (!file.eof())
	{
		glm::vec3 vec;
		file.getline(buffer.get(), lineSize, delimCharacter);
		line = buffer.get();

		if (line != "")
		{
			size_t pos = line.find_first_of(' ');
			std::string xVal = line.substr(0, pos);
			line = line.substr(pos + 1);
			pos = line.find_first_of(' ');
			std::string yVal = line.substr(0, pos);
			line = line.substr(pos + 1);
			pos = line.find_first_of(' ');
			std::string zVal = line.substr(0, pos);

			normals.push_back(glm::vec3(std::stof(xVal), std::stof(yVal), std::stof(zVal)));
		}
	}
}

void HitboxObject::calcHitboxCollisionRadius()
{
	float dist = 0.0f;
	float candidate = 0.0f;

	for(int i = 0; i < 8 - 1; ++i)
	{
		for (int j = i + 1; j < 8; ++j)
		{
			candidate = glm::length(hitbox->basicVertices[i] - hitbox->basicVertices[j]);
			if (candidate > dist)
				dist = candidate;
		}
	}

	basicCollisionRadius = dist / 2.0f;
}

void HitboxObject::load(HitboxPtr hitbox, const HitboxObjectData& filesData, const ModelExternalUniforms& uniforms)
{
	this->hitbox = hitbox;

	initializeData(filesData, uniforms);
}

void HitboxObject::initializeData(const HitboxObjectData& filesData, const ModelExternalUniforms& uniforms)
{
	renderer->loadShader(filesData.vertexShaderFilename, filesData.fragmentShaderFilename);
	initUniforms(uniforms);
	initAttribPointers();
	renderer->init();
	loadBufferData();
	calcHitboxCollisionRadius();
}

void HitboxObject::initUniforms(const ModelExternalUniforms& uniforms)
{
	UniformDataMat4Ptr modelUniform = std::make_shared<UniformDataMat4>("model");
	UniformDataMat4Ptr viewUniform = std::make_shared<UniformDataMat4>("view");
	UniformDataMat4Ptr projectionUniform = std::make_shared<UniformDataMat4>("projection");
	UniformDataVec3Ptr colorUniform = std::make_shared<UniformDataVec3>("color");

	modelUniform->mat = modelTransform;
	viewUniform->mat = uniforms.view;
	projectionUniform->mat = uniforms.projection;
	colorUniform->vec = hitboxColor;

	renderer->addUniform(modelUniform);
	renderer->addUniform(viewUniform);
	renderer->addUniform(projectionUniform);
	renderer->addUniform(colorUniform);
}

void HitboxObject::initAttribPointers()
{
	ShaderAttribute posAttrib;
	posAttrib.location = 0;
	posAttrib.size = 3;
	posAttrib.offset = (void*)0;

	renderer->addShaderAttribute(posAttrib);
}

void HitboxObject::loadBufferData()
{
	std::vector<float> buffer;

	for (int i = 0; i < 8; ++i)		//8 vertices in hitbox
	{
		for(int j=0;j<3;++j)		//3 components of vector
			buffer.push_back(hitbox->basicVertices[i][j]);
	}

	renderer->loadBuffer(buffer, hitbox->indices);
}

HitboxRendererPtr HitboxObject::getRenderer()const
{
	return renderer;
}

HitboxPtr HitboxObject::getHitbox()const
{
	return hitbox;
}

void HitboxObject::process()
{
	GameObject::process();
	*modelTransform = parentTransform * getTransform().getTransformMat();

	updateHitboxData();
}

void HitboxObject::updateHitboxData()
{
	for (int i = 0; i < 8; ++i)
	{
		hitbox->transformVertices[i] = (*modelTransform) * glm::vec4(hitbox->basicVertices[i], 1.0f);
	}
	for (int i = 0; i < 6; ++i)
	{
		hitbox->transformNormals[i] = (*modelTransform) * glm::vec4(hitbox->basicNormals[i], 0.0f);
	}

	glm::vec4 transformedColRadiusVec = (*modelTransform) * glm::vec4((radiusIdentityVec * basicCollisionRadius), 0.0f);
	actualCollisionRadius = glm::length(transformedColRadiusVec);
}

void HitboxObject::deepCopy(const HitboxObject& object)
{
	GameObject::deepCopy(object);

	this->renderer->deepCopy(*object.renderer);

	copyHitboxData(object.getHitbox());

	this->modelTransform = std::make_shared<glm::mat4>(1.0f);
	this->hitboxColor = std::make_shared<glm::vec3>(1.0f, 0.0f, 0.0f);
	this->basicCollisionRadius = object.basicCollisionRadius;

	copyModelUniform();
	copyColorUniform();
}

void HitboxObject::copyHitboxData(HitboxPtr hitbox)
{
	for (int i = 0; i < 8; ++i)
	{
		this->hitbox->basicVertices[i] = hitbox->basicVertices[i];
		this->hitbox->transformVertices[i] = hitbox->transformVertices[i];
	}

	for (int i = 0; i < 6; ++i)
	{
		this->hitbox->basicNormals[i] = hitbox->basicNormals[i];
		this->hitbox->transformNormals[i] = hitbox->transformNormals[i];
	}

	this->hitbox->indices = hitbox->indices;

	this->hitbox->damageMultiplier = hitbox->damageMultiplier;
}

void HitboxObject::copyModelUniform()
{
	UniformDataPtr sourceModelUniform = this->renderer->getUniform("model");
	int location = sourceModelUniform->location;
	this->renderer->deleteUniform("model");
	UniformDataMat4Ptr modelUniform = std::make_shared<UniformDataMat4>("model");
	modelUniform->mat = this->modelTransform;
	modelUniform->location = location;
	this->renderer->addUniform(modelUniform);
}

void HitboxObject::copyColorUniform()
{
	UniformDataPtr sourceColorUniform = this->renderer->getUniform("color");
	int location = sourceColorUniform->location;
	this->renderer->deleteUniform("color");
	UniformDataVec3Ptr colorUniform = std::make_shared<UniformDataVec3>("color");
	colorUniform->vec = this->hitboxColor;
	colorUniform->location = location;
	this->renderer->addUniform(colorUniform);
}

void HitboxObject::setActive(bool val)
{
	GameObject::setActive(val);

	renderer->setActive(val);
}

void HitboxObject::setHitboxColor(const glm::vec3& color)
{
	*hitboxColor = color;
}

glm::vec3 HitboxObject::getHitboxColor()const
{
	return *hitboxColor;
}

float HitboxObject::getCollisionRadius()const
{
	return actualCollisionRadius;
}

glm::vec3 HitboxObject::getGlobalPosition()const
{
	return ((*modelTransform) * glm::vec4(transform.getPosition(),1.0f));
}

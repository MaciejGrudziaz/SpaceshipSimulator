#pragma once

#include <GameResources/gameobjects/gameobject.h>
#include <GameResources/files/mgrimportfile.h>
#include <GameRenderer/texturerenderobject.h>
#include "modelimportstruct.h"
#include "hitboxobject.h"
#include "../utilities/time.h"

class StandardGameObject: public GameObject
{
public:
	enum error { NO_ERROR, MODEL_FILE_ERROR, TEXTURE_FILE_ERROR, VERTEX_SHADER_FILE_ERROR, FRAGMENT_SAHDER_FILE_ERROR };

	StandardGameObject();
	virtual ~StandardGameObject()
	{}

	void deepCopy(const StandardGameObject& object);

	virtual void load(const ModelData& data, const ModelExternalUniforms& uniforms);

	virtual void process() override;
	virtual void setActive(bool val) override;

	virtual void restart()
	{}

	virtual void dealDamage(float val);
	virtual void destroy()
	{}

	std::shared_ptr<RenderObject> getRenderer()const;
	HitboxPtr getMainHitbox()const;

	error getErrorCode();

protected:
	std::shared_ptr<RenderObject> renderer;
	std::shared_ptr<Hitbox> mainHitbox;
	std::shared_ptr<glm::mat4> modelTransform;
	std::shared_ptr<glm::vec3> hitColor;
	std::shared_ptr<float> currHitTime;
	std::shared_ptr<float> maxHitTime;
	bool textureAvailable;

	std::stack<error> errorCode;

	virtual void initializeRenderer();

	virtual void loadModel(const std::string& modelFilename);

	virtual void loadShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	virtual void loadStandardShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);
	virtual void loadTextureShaders(const std::string& vertexShaderFilename, const std::string& fragmentShaderFilename);

	virtual void initUniforms(const ModelExternalUniforms& uniforms);

	virtual void initAttribPointers();
	virtual void initStandardAttribPointers();
	virtual void initTextureAttribPointers();

	virtual void loadBufferData();
	virtual void loadStandardBufferData();
	virtual void loadTextureBufferData();

	virtual void loadTexture(const std::string& textureFilename);

	void checkTextureFile(const std::string& texFilename);

	void copyModelUniform();
	void copyHitColorUniforms();
};

typedef std::shared_ptr<StandardGameObject> StandardGameObjectPtr;

#pragma once

#include <GameResources/gameobjects/gameobject.h>
#include <GameResources/gameobjects/hitbox.h>
#include <GameRenderer/renderobject.h>
#include "../renderers/hitboxrenderer.h"
#include "modelimportstruct.h"

class HitboxObject: public GameObject
{
public:
	HitboxObject();

	void load(const HitboxObjectData& filesData, const ModelExternalUniforms& uniform);
	void load(HitboxPtr hitbox, const HitboxObjectData& filesData, const ModelExternalUniforms& uniforms);

	HitboxRendererPtr getRenderer()const;
	HitboxPtr getHitbox()const;

	virtual void process() override;

	void deepCopy(const HitboxObject& object);

	void setActive(bool val);

	void setHitboxColor(const glm::vec3& color);
	glm::vec3 getHitboxColor()const;

	float getCollisionRadius()const;

	glm::vec3 getGlobalPosition()const;

private:
	HitboxRendererPtr renderer;
	HitboxPtr hitbox;
	std::shared_ptr<glm::mat4> modelTransform;
	std::shared_ptr<glm::vec3> hitboxColor;

	const glm::vec3 radiusIdentityVec = glm::normalize(glm::vec3(1.0f, 1.0f, 1.0f));
	float basicCollisionRadius;
	float actualCollisionRadius;

	void loadHitboxFromfile(const std::string& hitboxFilename);
	bool isHitboxFileCorrect(const std::string& hitboxFilename);
	void loadControlPoints(std::fstream& file, std::vector<glm::vec3>& controlPt, const int lineSize, char delimCharacter);
	void loadIndices(std::fstream& file, std::vector<unsigned>& indices, const int lineSize, char delimCharacter);
	void loadNormals(std::fstream& file, std::vector<glm::vec3>& normals, const int lineSize, char delimCharacter);
	void calcHitboxCollisionRadius();

	void initializeData(const HitboxObjectData& filesData, const ModelExternalUniforms& uniform);
	void initUniforms(const ModelExternalUniforms& uniform);
	void initAttribPointers();
	void loadBufferData();

	void copyModelUniform();
	void copyColorUniform();

	void updateHitboxData();
	void copyHitboxData(HitboxPtr hitbox);
};

typedef std::shared_ptr<HitboxObject> HitboxObjectPtr;

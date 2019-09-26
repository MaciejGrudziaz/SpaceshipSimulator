#pragma once

#include <GameResources/gameobjects/gameobject.h>
#include <GameResources/files/mgrimportfile.h>
#include <GameRenderer/texturerenderobject.h>

class Spaceship: public GameObject
{
public:
	enum error { NO_ERROR, FILE_DATA_ERROR };

	Spaceship();

	void load(const std::string& filename);
	void loadShaders(const std::string& vertexShader, const std::string& fragmentShader);
	void loadTextureShaders(const std::string& vertexShader, const std::string& fragmentShader);

	void initRenderer(ConstMat4Ptr view, ConstMat4Ptr projection);
	void initTextureRenderer(ConstMat4Ptr view, ConstMat4Ptr projection);

	void loadBufferData();
	void loadTextureBufferData();

	void loadTexture(const std::string& filename);

	std::shared_ptr<RenderObject> getRenderer()const;
	std::shared_ptr<TextureRenderObject> getTextureRenderer()const;

	error getErrorCode();

private:
	std::shared_ptr<RenderObject> renderer;
	std::shared_ptr<TextureRenderObject> texRenderer;
	std::shared_ptr<Hitbox> mainHitbox;
	std::shared_ptr<glm::mat4> modelTransform;

	std::stack<error> errorCode;
};

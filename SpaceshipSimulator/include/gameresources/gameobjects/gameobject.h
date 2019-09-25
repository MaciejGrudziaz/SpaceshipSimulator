#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <list>
#include <algorithm>
#include "property.h"
#include "basicobject.h"
#include "objecttransform.h"

struct GameObject
{
public:
	typedef property_map<GameObject>::value PropertyMap;
	typedef std::map<std::string, std::shared_ptr<GameObject> > GameObjectMap;
	typedef std::map<std::string, std::shared_ptr<BasicObject> > ModelMap;

	GameObject();
	GameObject(const std::string& name);
	virtual ~GameObject();

	void setActive(bool val);
	bool isActive() const;
	void setName(const std::string& name);
	std::string getName()const;
	ObjectTransform& getTransform();

	template<class T>
	void addProperty(const std::string& name);
	std::list<std::string> getPropertiesNames() const;
	bool removeProperty(const std::string& name);

	void addChild(std::shared_ptr<GameObject> child);
	void removeChild(std::string name);
	std::list<std::string> getChildrenNames() const;
	std::shared_ptr<GameObject> getChild(std::string name);

	void addModel(BasicObjectPtr model);
	void removeModel(std::string name);
	std::list<std::string> getModelsNames() const;
	BasicObjectPtr getModel(std::string name);

	virtual bool isUsable()const;

	virtual void init();
	virtual void process();
	virtual void invalidate();

protected:
	ObjectTransform transform;
	bool active;
	std::string name;

	PropertyMap properties;

	GameObjectMap children;

	ModelMap models;

	bool isChildNameUnique(std::string name);
	bool isModelNameUnique(std::string name);
};

typedef std::shared_ptr<GameObject> GameObjectSPtr;

template<class T>
void GameObject::addProperty(const std::string& name)
{
	//if class T inherits from Property<GameObject> then add an object of this class to GameObject
	register_property<GameObject, T>::add_to_map::add(properties, name, *this);
}

struct NullGameObject : public GameObject
{
	bool isUsable()const override
	{
		return false;
	}
};

#endif GAMEOBJECT_H
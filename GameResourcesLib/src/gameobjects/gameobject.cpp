#include <stdafx.h>

GameObject::GameObject()
	: active(true)
	, name("")
{}

GameObject::GameObject(const std::string& name)
	: active(true)
	, name(name)
{}

GameObject::~GameObject()
{}

void GameObject::setActive(bool val)
{
	active = val;
}

bool GameObject::isActive() const
{
	return active;
}

void GameObject::setName(const std::string& name)
{
	this->name = name;
}

std::string GameObject::getName()const
{
	return name;
}

ObjectTransform& GameObject::getTransform()
{
	return transform;
}

std::list<std::string> GameObject::getPropertiesNames() const
{
	std::list<std::string> propertiesNames;

	std::for_each(properties.begin(), properties.end(),
		[&propertiesNames](auto propertyPair) { propertiesNames.push_back(propertyPair.first); });

	return propertiesNames;
}

bool GameObject::removeProperty(const std::string& name)
{
	PropertyMap::size_type del = properties.erase(name);

	if (del > 0)
		return true;
	else return false;
}

void GameObject::addChild(std::shared_ptr<GameObject> child)
{
	if (child->name != "" && isChildNameUnique(child->name))
	{
		children.insert(GameObjectMap::value_type(child->name, child));
	}
}

bool GameObject::isChildNameUnique(std::string name)
{
	bool isUnique = true;

	if (this->name == name)
	{
		isUnique = false;
	}

	if (isUnique)
	{
		GameObjectMap::iterator it = children.find(name);
		if (it != children.end())
		{
			isUnique = false;
		}
	}

	return isUnique;
}

void GameObject::removeChild(std::string name)
{
	children.erase(name);
}

std::list<std::string> GameObject::getChildrenNames() const
{
	std::list<std::string> list;
	std::for_each(children.begin(), children.end(),
		[&list](auto childPair) { list.push_back(childPair.first); });

	return list;
}

std::shared_ptr<GameObject> GameObject::getChild(std::string name)
{
	GameObjectMap::iterator it = children.find(name);

	if (it != children.end())
	{
		return it->second;
	}
	else {
		return std::make_shared<NullGameObject>();
	}
}

void GameObject::addModel(BasicObjectPtr model)
{
	if (model->name != "" && isModelNameUnique(model->name))
	{
		models.insert(ModelMap::value_type(model->name, model));
	}
}

bool GameObject::isModelNameUnique(std::string name)
{
	auto modelIt = models.find(name);

	return (modelIt != models.end()) ? false : true;
}

void GameObject::removeModel(std::string name)
{
	models.erase(name);
}

std::list<std::string> GameObject::getModelsNames() const
{
	std::list<std::string> list;

	std::for_each(models.begin(), models.end(),
		[&list](auto modelPair) {list.push_back(modelPair.first); });

	return list;
}

BasicObjectPtr GameObject::getModel(std::string name)
{
	auto modelIt = models.find(name);

	if (modelIt != models.end())
	{
		return modelIt->second;
	}
	else {
		return std::make_shared<NullBasicObject>();
	}
}

bool GameObject::isUsable()const
{
	return true;
}

void GameObject::init()
{
	std::for_each(properties.begin(), properties.end(), [](auto property) { property.second->init(); });
}

void GameObject::process()
{
	std::for_each(properties.begin(), properties.end(), [](auto property) { property.second->process(); });
}

void GameObject::invalidate()
{
	std::for_each(properties.begin(), properties.end(), [](auto property) { property.second->invalidate(); });
}
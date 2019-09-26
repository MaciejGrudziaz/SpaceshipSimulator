#ifndef PROPERTY_H
#define PROPERTY_H

#include <string>
#include <map>

template <class ObjectType>
class Property
{
protected:
	ObjectType & object;
	std::string name;

public:
	Property(const std::string& name, ObjectType& object)
		: object(object)
		, name(name)
	{}

	virtual ~Property()
	{}
	virtual void init() = 0;
	virtual void process() = 0;
	virtual void invalidate() = 0;
};

//template classes to help implementing Properties inside other classes
//taking into cnsideration all necessary constraints

template<class ObjectType>
struct property_map
{
	typedef std::map<std::string, std::shared_ptr<Property<ObjectType> > > value;
};

template<bool test, class ObjectType, class PropertyType>
struct add_to_map;

template<class ObjectType, class PropertyType>
struct add_to_map<true, ObjectType, PropertyType>
{
	typedef typename property_map<ObjectType>::value PropertyMap;

	static void add(PropertyMap& properties, const std::string& name, ObjectType& object)
	{
		std::shared_ptr<Property<ObjectType> > property = std::make_shared<PropertyType>(name, object);
		properties.insert(PropertyMap::value_type(name, property));
	}
};

template<class ObjectType, class PropertyType>
struct add_to_map<false, ObjectType, PropertyType>
{
	typedef typename property_map<ObjectType>::value PropertyMap;

	static void add(PropertyMap& properties, const std::string& name, ObjectType& object)
	{
		static_assert(false, "GameObject::add_to_map::add: property type should be derived from Property<GameObject> class");
	}
};

template<class ObjectType, class PropertyType>
struct register_property
{
	typedef typename add_to_map<std::is_base_of<Property<ObjectType>, PropertyType>::value, ObjectType, PropertyType> add_to_map;
};

#endif // PROPERTY_H

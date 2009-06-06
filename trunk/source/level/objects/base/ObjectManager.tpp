/***
 * hesperus: ObjectManager.tpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include <boost/lexical_cast.hpp>
#include <boost/pointer_cast.hpp>

#include <source/exceptions/Exception.h>

namespace hesp {

template <typename T>
shared_ptr<T> ObjectManager::get_component(const ObjectID& id, const shared_ptr<T>&)
{
	return boost::const_pointer_cast<T,const T>(const_cast<const ObjectManager *>(this)->get_component<const T>(id));
}

template <typename T>
shared_ptr<const T> ObjectManager::get_component(const ObjectID& id, const shared_ptr<const T>&) const
{
	std::map<ObjectID,Object>::const_iterator it = m_objects.find(id);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + id.to_string());

	const Object& object = it->second;

	Object::const_iterator jt = object.find(T::static_group_type());
	if(jt == object.end()) return shared_ptr<const T>();

	// Note that there's an implicit T -> const T conversion done when returning here.
	return boost::dynamic_pointer_cast<T,IObjectComponent>(jt->second);
}

template <typename T>
void ObjectManager::set_component(const ObjectID& id, const shared_ptr<T>& component)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(id);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + boost::lexical_cast<std::string,int>(id.value()));

	Object& object = it->second;
	object[T::static_group_type()] = component;
}

}

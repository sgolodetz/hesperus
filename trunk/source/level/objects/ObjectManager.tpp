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
	std::map<ObjectID,Object>::iterator it = m_objects.find(id);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + boost::lexical_cast<std::string,int>(id.value()));

	Object& object = it->second;

	Object::iterator jt = object.find(T::static_type());
	if(jt == object.end()) return shared_ptr<T>();

	return boost::dynamic_pointer_cast<T,IComponent>(jt->second);
}

template <typename T>
void ObjectManager::set_component(const ObjectID& id, const shared_ptr<T>& component)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(id);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + boost::lexical_cast<std::string,int>(id.value()));

	Object& object = it->second;
	object[T::static_type()] = component;
}

}

/***
 * hesperus: ObjectManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectManager.h"

#include "MsgObjectDestroyed.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
ObjectManager::ObjectManager(const std::vector<AABB3d>& aabbs, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes)
:	m_aabbs(aabbs), m_componentPropertyTypes(componentPropertyTypes)
{}

//#################### PUBLIC METHODS ####################
const std::vector<AABB3d>& ObjectManager::aabbs() const
{
	return m_aabbs;
}

void ObjectManager::broadcast_immediate_message(const Message_CPtr& msg)
{
	for(std::map<ObjectID,Object>::iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
	{
		post_message_to_object(it->second, msg);
	}
}

const std::map<std::string,std::map<std::string,std::string> >& ObjectManager::component_property_types() const
{
	return m_componentPropertyTypes;
}

void ObjectManager::consolidate_object_ids()
{
	// TODO:	This only needs implementing if I want to save objects in-game. If I only save objects I just
	//			loaded in (e.g. as part of the level-making process), their IDs will already be contiguous.
}

ObjectID ObjectManager::create_object(const std::vector<IComponent_Ptr>& components)
{
	ObjectID id(m_idAllocator.allocate());
	Object& object = m_objects[id];

	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		components[i]->set_object_id(id);
		components[i]->set_object_manager(this);
		object.insert(std::make_pair(components[i]->type(), components[i]));
	}

	return id;
}

void ObjectManager::destroy_object(const ObjectID& id)
{
	m_idAllocator.deallocate(id.value());
	m_objects.erase(id);
	broadcast_immediate_message(Message_CPtr(new MsgObjectDestroyed(id)));
}

std::vector<IComponent_Ptr> ObjectManager::get_components(const ObjectID& id)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(id);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + boost::lexical_cast<std::string,int>(id.value()));

	Object& object = it->second;

	std::vector<IComponent_Ptr> components;
	for(Object::const_iterator jt=object.begin(), jend=object.end(); jt!=jend; ++jt)
	{
		components.push_back(jt->second);
	}
	return components;
}

int ObjectManager::object_count() const
{
	return static_cast<int>(m_objects.size());
}

void ObjectManager::post_immediate_message(const ObjectID& target, const Message_CPtr& msg)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(target);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + boost::lexical_cast<std::string,int>(target.value()));
	post_message_to_object(it->second, msg);
}

//#################### PRIVATE METHODS ####################
void ObjectManager::post_message_to_object(Object& target, const Message_CPtr& msg)
{
	for(Object::iterator jt=target.begin(), jend=target.end(); jt!=jend; ++jt)
	{
		msg->dispatch(jt->second.get());
	}
}

}

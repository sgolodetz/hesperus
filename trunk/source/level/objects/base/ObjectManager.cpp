/***
 * hesperus: ObjectManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectManager.h"

#include <source/level/objects/components/ICmpActivatable.h>
#include <source/level/objects/components/ICmpModelRender.h>
#include <source/level/objects/components/ICmpOwnable.h>
#include <source/level/objects/components/ICmpPhysics.h>
#include <source/level/objects/components/ICmpYoke.h>
#include <source/level/objects/messages/MsgObjectDestroyed.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
ObjectManager::ObjectManager(const std::vector<AABB3d>& aabbs, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes)
:	m_aabbs(aabbs), m_componentPropertyTypes(componentPropertyTypes)
{
	register_group("Activatables", is_activatable);
	register_group("Animatables", is_animatable);
	register_group("Renderables", is_renderable);
	register_group("Simulables", is_simulable);
	register_group("Yokeables", is_yokeable);
}

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

ObjectID ObjectManager::create_object(const std::vector<IObjectComponent_Ptr>& components)
{
	ObjectID id(m_idAllocator.allocate());
	Object& object = m_objects[id];

	// Add the components to the object manager.
	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		components[i]->set_object_id(id);
		components[i]->set_object_manager(this);
		object.insert(std::make_pair(components[i]->group_type(), components[i]));
	}

	// Check component dependencies (note that this must happen after all the components have been added above).
	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		components[i]->check_dependencies();
	}

	return id;
}

void ObjectManager::destroy_object(const ObjectID& id)
{
	m_idAllocator.deallocate(id.value());
	m_objects.erase(id);
	broadcast_immediate_message(Message_CPtr(new MsgObjectDestroyed(id)));
}

std::vector<IObjectComponent_Ptr> ObjectManager::get_components(const ObjectID& id)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(id);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + id.to_string());

	Object& object = it->second;

	std::vector<IObjectComponent_Ptr> components;
	for(Object::const_iterator jt=object.begin(), jend=object.end(); jt!=jend; ++jt)
	{
		components.push_back(jt->second);
	}
	return components;
}

std::vector<ObjectID> ObjectManager::group(const std::string& name) const
{
	std::vector<ObjectID> ret;

	std::map<std::string,GroupPredicate>::const_iterator gt = m_groupPredicates.find(name);
	if(gt == m_groupPredicates.end()) throw Exception("No such object group: " + name);
	const GroupPredicate& pred = gt->second;

	for(std::map<ObjectID,Object>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
	{
		const ObjectID& objectID = it->first;
		if(pred(objectID, this)) ret.push_back(objectID);
	}

	return ret;
}

int ObjectManager::object_count() const
{
	return static_cast<int>(m_objects.size());
}

ObjectID ObjectManager::player() const
{
	// FIXME: The object ID of the player should be loaded in, not hard-coded like this.
	return ObjectID(0);
}

void ObjectManager::post_immediate_message(const ObjectID& target, const Message_CPtr& msg)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(target);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + target.to_string());
	post_message_to_object(it->second, msg);
}

void ObjectManager::register_group(const std::string& name, const GroupPredicate& pred)
{
	m_groupPredicates[name] = pred;
}

//#################### PRIVATE METHODS ####################
bool ObjectManager::has_owner(const ObjectID& objectID, const ObjectManager *objectManager)
{
	ICmpOwnable_CPtr cmpOwnable = objectManager->get_component(objectID, cmpOwnable);
	return cmpOwnable && cmpOwnable->owner().valid();
}

bool ObjectManager::is_activatable(const ObjectID& objectID, const ObjectManager *objectManager)
{
	return !has_owner(objectID, objectManager) && objectManager->get_component<ICmpActivatable>(objectID) != NULL;
}

bool ObjectManager::is_animatable(const ObjectID& objectID, const ObjectManager *objectManager)
{
	return !has_owner(objectID, objectManager) && objectManager->get_component<ICmpModelRender>(objectID) != NULL;
}

bool ObjectManager::is_renderable(const ObjectID& objectID, const ObjectManager *objectManager)
{
	return !has_owner(objectID, objectManager) && objectManager->get_component<ICmpRender>(objectID) != NULL;
}

bool ObjectManager::is_simulable(const ObjectID& objectID, const ObjectManager *objectManager)
{
	return !has_owner(objectID, objectManager) && objectManager->get_component<ICmpPhysics>(objectID) != NULL;
}

bool ObjectManager::is_yokeable(const ObjectID& objectID, const ObjectManager *objectManager)
{
	return !has_owner(objectID, objectManager) && objectManager->get_component<ICmpYoke>(objectID) != NULL;
}

void ObjectManager::post_message_to_object(Object& target, const Message_CPtr& msg)
{
	for(Object::iterator jt=target.begin(), jend=target.end(); jt!=jend; ++jt)
	{
		msg->dispatch(jt->second.get());
	}
}

}

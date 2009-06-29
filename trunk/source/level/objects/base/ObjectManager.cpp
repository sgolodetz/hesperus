/***
 * hesperus: ObjectManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "ObjectManager.h"

#include <source/level/objects/components/ICmpActivatable.h>
#include <source/level/objects/components/ICmpInventory.h>
#include <source/level/objects/components/ICmpModelRender.h>
#include <source/level/objects/components/ICmpOwnable.h>
#include <source/level/objects/components/ICmpPhysics.h>
#include <source/level/objects/components/ICmpYoke.h>
#include <source/level/objects/messages/MsgObjectDestroyed.h>
#include <source/level/objects/messages/MsgObjectPredestroyed.h>

namespace hesp {

//#################### LOCAL METHODS - DECLARATIONS ####################
bool has_owner(const ObjectID& id, const ObjectManager *objectManager);
bool is_activatable(const ObjectID& id, const ObjectManager *objectManager);
bool is_animatable(const ObjectID& id, const ObjectManager *objectManager);
bool is_model_container(const ObjectID& id, const ObjectManager *objectManager);
bool is_renderable(const ObjectID& id, const ObjectManager *objectManager);
bool is_simulable(const ObjectID& id, const ObjectManager *objectManager);
bool is_yokeable(const ObjectID& id, const ObjectManager *objectManager);

//#################### CONSTRUCTORS ####################
ObjectManager::ObjectManager(const std::vector<AABB3d>& aabbs, const std::map<std::string,std::map<std::string,std::string> >& componentPropertyTypes)
:	m_aabbs(aabbs), m_componentPropertyTypes(componentPropertyTypes)
{
	register_group("Activatables", is_activatable);
	register_group("Animatables", is_animatable);
	register_group("ModelContainers", is_model_container);
	register_group("Renderables", is_renderable);
	register_group("Simulables", is_simulable);
	register_group("Yokeables", is_yokeable);
}

//#################### PUBLIC METHODS ####################
const std::vector<AABB3d>& ObjectManager::aabbs() const
{
	return m_aabbs;
}

/**
Informs the object manager that the specified component is interested in receiving
broadcasted messsages about the specified object.

@param listener	The component which wants to receive object messages
@param id		The ID of the object about which the component wants to receive messages
*/
void ObjectManager::add_listener(IObjectComponent *listener, const ObjectID& id)
{
	m_listenerTable.add_listener(listener->object_id(), listener->group_type(), id);
}

void ObjectManager::broadcast_message(const Message_CPtr& msg)
{
	std::set<ObjectID> msgObjs = msg->referenced_objects();
	for(std::set<ObjectID>::const_iterator it=msgObjs.begin(), iend=msgObjs.end(); it!=iend; ++it)
	{
		std::vector<std::pair<ObjectID,std::string> > listeners = m_listenerTable.get_listeners(*it);
		for(size_t j=0, size=listeners.size(); j<size; ++j)
		{
			IObjectComponent_Ptr listener = get_component<IObjectComponent>(listeners[j].first, listeners[j].second);
			if(listener) msg->dispatch(listener.get());
		}
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

	// Check component dependencies and register listening (note that these must happen
	// after all the components have been added above).
	for(size_t i=0, size=components.size(); i<size; ++i)
	{
		components[i]->check_dependencies();
		components[i]->register_listening();
	}

	return id;
}

void ObjectManager::flush_destruction_queue()
{
	typedef DestructionQueue::Element Elt;

	DestructionQueue& q = m_destructionQueue;

	while(!q.empty())
	{
		Elt& e = q.top();
		ObjectID id = e.id();
		bool& predestroyFlag = e.data();

		if(predestroyFlag)
		{
			// The pre-destroy message has already been sent for this object.
			destroy_object(id);
			q.pop();
		}
		else
		{
			// Note: The flag must be set before sending the pre-destroy message, which may result in queue changes.
			predestroyFlag = true;
			broadcast_message(Message_CPtr(new MsgObjectPredestroyed(id)));
		}
	}
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

void ObjectManager::post_message(const ObjectID& target, const Message_CPtr& msg)
{
	std::map<ObjectID,Object>::iterator it = m_objects.find(target);
	if(it == m_objects.end()) throw Exception("Invalid object ID: " + target.to_string());

	Object& obj = it->second;
	for(Object::iterator jt=obj.begin(), jend=obj.end(); jt!=jend; ++jt)
	{
		msg->dispatch(jt->second.get());
	}
}

void ObjectManager::queue_child_for_destruction(const ObjectID& child, const ObjectID& parent)
{
	int parentPriority = m_destructionQueue.element(parent).key();
	m_destructionQueue.insert(child, parentPriority+1, false);
}

void ObjectManager::queue_for_destruction(const ObjectID& id)
{
	m_destructionQueue.insert(id, 0, false);
}

void ObjectManager::register_group(const std::string& name, const GroupPredicate& pred)
{
	m_groupPredicates[name] = pred;
}

/**
Informs the object manager that the specified component is no longer interested
in receiving broadcasted messsages about the specified object.

@param listener	The component which no longer wants to receive object messages
@param id		The ID of the object about which the component no longer wants to receive messages
*/
void ObjectManager::remove_listener(IObjectComponent *listener, const ObjectID& id)
{
	m_listenerTable.remove_listener(listener->object_id(), listener->group_type(), id);
}

//#################### PRIVATE METHODS ####################
void ObjectManager::destroy_object(const ObjectID& id)
{
	// Remove all the listeners which are components of the object being deleted.
	m_listenerTable.remove_listeners_from(id);

	broadcast_message(Message_CPtr(new MsgObjectDestroyed(id)));

	// Remove all the listeners referring to the object.
	m_listenerTable.remove_listeners_to(id);

	m_objects.erase(id);
	m_idAllocator.deallocate(id.value());
}

//#################### LOCAL METHODS - DEFINITIONS ####################
bool has_owner(const ObjectID& id, const ObjectManager *objectManager)
{
	ICmpOwnable_CPtr cmpOwnable = objectManager->get_component(id, cmpOwnable);
	return cmpOwnable && cmpOwnable->owner().valid();
}

bool is_activatable(const ObjectID& id, const ObjectManager *objectManager)
{
	return !has_owner(id, objectManager) && objectManager->get_component<ICmpActivatable>(id) != NULL;
}

bool is_animatable(const ObjectID& id, const ObjectManager *objectManager)
{
	if(objectManager->get_component<ICmpModelRender>(id))
	{
		ICmpOwnable_CPtr cmpOwnable = objectManager->get_component(id, cmpOwnable);
		if(cmpOwnable && cmpOwnable->owner().valid())
		{
			ICmpInventory_CPtr cmpOwnerInventory = objectManager->get_component(cmpOwnable->owner(), cmpOwnerInventory);

			// Even if the object is owned by another object, it still needs animating if it's the currently active item.
			return cmpOwnerInventory->active_item() == id;
		}
		else return true;		// the object has a ModelRender component, and is either not ownable, or not currently owned
	}
	else return false;			// animatable objects must have a ModelRender component
}

bool is_model_container(const ObjectID& id, const ObjectManager *objectManager)
{
	return objectManager->get_component<ICmpModelRender>(id) != NULL;
}

bool is_renderable(const ObjectID& id, const ObjectManager *objectManager)
{
	return !has_owner(id, objectManager) && objectManager->get_component<ICmpRender>(id) != NULL;
}

bool is_simulable(const ObjectID& id, const ObjectManager *objectManager)
{
	return !has_owner(id, objectManager) && objectManager->get_component<ICmpPhysics>(id) != NULL;
}

bool is_yokeable(const ObjectID& id, const ObjectManager *objectManager)
{
	return !has_owner(id, objectManager) && objectManager->get_component<ICmpYoke>(id) != NULL;
}

}

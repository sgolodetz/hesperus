/***
 * hesperus: CmpInventory.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpInventory.h"

#include <set>
#include <vector>

#include <source/level/objects/messages/MsgObjectDestroyed.h>
#include <source/level/objects/messages/MsgObjectPredestroyed.h>
#include "ICmpUsable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpInventory::CmpInventory(const ObjectID& activeObject, const std::map<std::string,int>& consumables, const std::set<ObjectID>& objects)
:	m_initialised(false), m_activeObject(activeObject), m_consumables(consumables), m_objects(objects)
{
	// Note:	We can't update the usable groups at this stage, because the objects we're holding generally haven't been created yet.
	//			Instead, they are initialised "on demand" later on.

	// Check that the active object (if any) is actually one of the objects in the inventory.
	if(activeObject.valid() && objects.find(activeObject) == objects.end())
	{
		throw Exception("The active object must be in the inventory");
	}
}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpInventory::load(const Properties& properties)
{
	// Convert the vector of object IDs stored in the properties into a set.
	const std::vector<ObjectID>& propObjects = properties.get<std::vector<ObjectID> >("Objects");
	std::set<ObjectID> objects(propObjects.begin(), propObjects.end());

	return IObjectComponent_Ptr(new CmpInventory(properties.get<ObjectID>("ActiveObject"), properties.get<std::map<std::string,int> >("Consumables"), objects));
}

//#################### PUBLIC METHODS ####################
ObjectID CmpInventory::active_object() const
{
	initialise_if_necessary();
	return m_activeObject;
}

void CmpInventory::add_consumables(const std::string& type, int amount)
{
	// Check precondition.
	if(amount < 0) throw Exception("Cannot add a negative number of consumables");

	std::map<std::string,int>::iterator it = m_consumables.find(type);
	if(it != m_consumables.end()) it->second += amount;
	else m_consumables.insert(std::make_pair(type, amount));
}

void CmpInventory::add_object(const ObjectID& id)
{
	initialise_if_necessary();

	m_objects.insert(id);
	m_objectManager->add_listener(this, id);

	ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(id, cmpUsable);
	if(cmpUsable) m_groups[cmpUsable->usable_group()].insert(id);
}

void CmpInventory::destroy_consumables(const std::string& type, int amount)
{
	// Check precondition.
	if(amount < 0) throw Exception("Cannot destroy a negative number of consumables");

	std::map<std::string,int>::iterator it = m_consumables.find(type);
	if(it != m_consumables.end() && it->second >= amount)
	{
		it->second -= amount;
	}
	else throw Exception("Cannot destroy more consumables than are being held");
}

void CmpInventory::process_message(const MsgObjectDestroyed& msg)
{
	if(m_objects.find(msg.object_id()) != m_objects.end())
	{
		// An object contained in the inventory has been destroyed: we need to remove it from the inventory.
		// Note that the object still exists until after processing of destroyed messages has finished (see
		// the implementation in ObjectManager), so there is no danger of accessing the object after it has
		// already been physically destroyed.
		remove_object(msg.object_id());
	}
}

void CmpInventory::process_message(const MsgObjectPredestroyed& msg)
{
	if(msg.object_id() == m_objectID)
	{
		// The object whose inventory this is is about to be destroyed. All the objects in the inventory
		// thus need to be queued up for prior destruction.
		for(std::set<ObjectID>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
		{
			m_objectManager->queue_child_for_destruction(*it, m_objectID);
		}
	}
}

void CmpInventory::register_listening()
{
	// Note: The inventory needs to receive predestroy messages for the object to which it belongs.
	m_objectManager->add_listener(this, m_objectID);

	for(std::set<ObjectID>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
	{
		m_objectManager->add_listener(this, *it);
	}
}

void CmpInventory::remove_object(const ObjectID& id)
{
	initialise_if_necessary();

	m_objects.erase(id);
	m_objectManager->remove_listener(this, id);
	if(m_activeObject == id) m_activeObject = ObjectID();

	ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(id, cmpUsable);
	if(cmpUsable) m_groups[cmpUsable->usable_group()].erase(id);
}

std::pair<std::string,Properties> CmpInventory::save() const
{
	Properties properties;

	// Convert the set of IDs of objects stored in the inventory into a vector.
	std::vector<ObjectID> propObjects(m_objects.begin(), m_objects.end());

	properties.set("ActiveObject", m_activeObject);
	properties.set("Consumables", m_consumables);
	properties.set("Objects", propObjects);

	return std::make_pair("Inventory", properties);
}

//#################### PRIVATE METHODS ####################
void CmpInventory::initialise_if_necessary() const
{
	if(!m_initialised)
	{
		for(std::set<ObjectID>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
		{
			ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(*it, cmpUsable);
			if(cmpUsable) m_groups[cmpUsable->usable_group()].insert(*it);
		}

		m_initialised = true;
	}
}

}

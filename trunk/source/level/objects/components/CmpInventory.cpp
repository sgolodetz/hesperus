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
CmpInventory::CmpInventory(const std::set<ObjectID>& objects, const std::map<std::string,int>& consumables)
:	m_objects(objects), m_consumables(consumables), m_groupsDirty(true)
{
	// Note: We can't update the usable groups at this stage, because the objects we're holding generally haven't been created yet.
}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpInventory::load(const Properties& properties)
{
	// Convert the [int] of objects stored in the properties into a set of object IDs.
	const std::vector<int>& propObjects = properties.get<std::vector<int> >("Objects");
	std::set<ObjectID> objects;
	for(std::vector<int>::const_iterator it=propObjects.begin(), iend=propObjects.end(); it!=iend; ++it)
	{
		objects.insert(ObjectID(*it));
	}

	return IObjectComponent_Ptr(new CmpInventory(objects, properties.get<std::map<std::string,int> >("Consumables")));
}

//#################### PUBLIC METHODS ####################
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
	m_objects.insert(id);
	m_objectManager->add_listener(this, id);

	if(!m_groupsDirty)
	{
		ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(id, cmpUsable);
		if(cmpUsable) m_groups[cmpUsable->usable_group()].insert(id);
	}
	else update_groups();
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
	m_objects.erase(id);
	m_objectManager->remove_listener(this, id);

	if(!m_groupsDirty)
	{
		ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(id, cmpUsable);
		if(cmpUsable) m_groups[cmpUsable->usable_group()].erase(id);
	}
	else update_groups();
}

std::pair<std::string,Properties> CmpInventory::save() const
{
	Properties properties;

	// Convert the set of IDs of objects stored in the inventory into an [int].
	std::vector<int> propObjects;
	propObjects.reserve(m_objects.size());
	for(std::set<ObjectID>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
	{
		propObjects.push_back(it->value());
	}

	properties.set("Objects", propObjects);
	properties.set("Consumables", m_consumables);

	return std::make_pair("Inventory", properties);
}

//#################### PRIVATE METHODS ####################
void CmpInventory::update_groups()
{
	m_groups.clear();

	for(std::set<ObjectID>::const_iterator it=m_objects.begin(), iend=m_objects.end(); it!=iend; ++it)
	{
		ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(*it, cmpUsable);
		if(cmpUsable) m_groups[cmpUsable->usable_group()].insert(*it);
	}

	m_groupsDirty = false;
}

}

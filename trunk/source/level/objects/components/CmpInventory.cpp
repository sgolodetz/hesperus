/***
 * hesperus: CmpInventory.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpInventory.h"

#include <set>
#include <vector>

#include <source/level/objects/base/ObjectManager.h>
#include "ICmpUsable.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpInventory::CmpInventory(const std::set<ObjectID>& objects, const std::map<std::string,int>& consumables)
:	m_objects(objects), m_consumables(consumables), m_groupsDirty(true)
{
	// Note: We can't update the usable groups at this stage, because the objects we're holding generally haven't been created yet.
}

//#################### STATIC FACTORY METHODS ####################
IComponent_Ptr CmpInventory::load(const Properties& properties)
{
	// Convert the [int] of objects stored in the properties into a set of object IDs.
	const std::vector<int>& propObjects = properties.get_actual<std::vector<int> >("Objects");
	std::set<ObjectID> objects;
	for(std::vector<int>::const_iterator it=propObjects.begin(), iend=propObjects.end(); it!=iend; ++it)
	{
		objects.insert(ObjectID(*it));
	}

	return IComponent_Ptr(new CmpInventory(objects, properties.get_actual<std::map<std::string,int> >("Consumables")));
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

void CmpInventory::add_object(const ObjectID& objectID)
{
	m_objects.insert(objectID);

	if(!m_groupsDirty)
	{
		ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(objectID, cmpUsable);
		if(cmpUsable) m_groups[cmpUsable->usable_group()].insert(objectID);
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

void CmpInventory::remove_object(const ObjectID& objectID)
{
	m_objects.erase(objectID);

	if(!m_groupsDirty)
	{
		ICmpUsable_Ptr cmpUsable = m_objectManager->get_component(objectID, cmpUsable);
		if(cmpUsable) m_groups[cmpUsable->usable_group()].erase(objectID);
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

	properties.set_actual("Objects", propObjects);
	properties.set_actual("Consumables", m_consumables);

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

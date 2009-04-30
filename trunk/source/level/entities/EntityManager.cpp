/***
 * hesperus: EntityManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityManager.h"

#include <source/exceptions/Exception.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
EntityManager::EntityManager(const std::vector<Entity_Ptr>& entities, const std::vector<AABB3d>& aabbs, const std::map<std::string,std::string>& propertyTypes)
:	m_entities(entities), m_aabbs(aabbs), m_propertyTypes(propertyTypes)
{
	// Add the entities to the relevant groups.
	std::vector<Entity_Ptr>& animatables = m_groups["Animatables"];
	std::vector<Entity_Ptr>& simulables = m_groups["Simulables"];
	std::vector<Entity_Ptr>& yokeables = m_groups["Yokeables"];
	int entityCount = static_cast<int>(m_entities.size());
	for(int i=0; i<entityCount; ++i)
	{
		if(entities[i]->is_animatable()) animatables.push_back(entities[i]);
		if(entities[i]->is_simulable()) simulables.push_back(entities[i]);
		if(entities[i]->yoke()) yokeables.push_back(entities[i]);

		if(entities[i]->archetype() == "Player")
		{
			if(!m_viewer) m_viewer = entities[i];
			else throw Exception("There can only be one viewer in a level");
		}
	}

	if(!m_viewer) throw Exception("The viewer must exist");
}

//#################### PUBLIC METHODS ####################
const std::vector<AABB3d>& EntityManager::aabbs() const
{
	return m_aabbs;
}

const std::vector<Entity_Ptr>& EntityManager::entities() const
{
	return m_entities;
}

const std::vector<Entity_Ptr>& EntityManager::group(const std::string& name) const
{
	std::map<std::string,std::vector<Entity_Ptr> >::const_iterator it = m_groups.find(name);
	if(it != m_groups.end()) return it->second;
	else throw Exception("No such entity group: " + name);
}

const std::map<std::string,std::string>& EntityManager::property_types() const
{
	return m_propertyTypes;
}

const Entity_Ptr& EntityManager::viewer() const
{
	return m_viewer;
}

}

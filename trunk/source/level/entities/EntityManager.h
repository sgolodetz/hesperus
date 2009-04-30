/***
 * hesperus: EntityManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYMANAGER
#define H_HESP_ENTITYMANAGER

#include <map>

#include "Entity.h"

namespace hesp {

class EntityManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<AABB3d> m_aabbs;
	std::vector<Entity_Ptr> m_entities;
	std::map<std::string,std::vector<Entity_Ptr> > m_groups;
	std::map<std::string,std::string> m_propertyTypes;
	Entity_Ptr m_viewer;

	//#################### CONSTRUCTORS ####################
public:
	EntityManager(const std::vector<Entity_Ptr>& entities, const std::vector<AABB3d>& aabbs, const std::map<std::string,std::string>& propertyTypes);

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<AABB3d>& aabbs() const;
	const std::vector<Entity_Ptr>& entities() const;
	const std::vector<Entity_Ptr>& group(const std::string& name) const;
	const std::map<std::string,std::string>& property_types() const;
	const Entity_Ptr& viewer() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityManager> EntityManager_Ptr;
typedef shared_ptr<const EntityManager> EntityManager_CPtr;

}

#endif

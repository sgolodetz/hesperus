/***
 * hesperus: EntityManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYMANAGER
#define H_HESP_ENTITYMANAGER

#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>

#include <source/math/geom/AABB.h>
#include "Entity.h"

namespace hesp {

class EntityManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_entDefFilename;

	std::vector<AABB3d> m_aabbs;
	std::vector<Entity_Ptr> m_entities;
	std::vector<Entity_Ptr> m_simulables;	// entities which should undergo physical simulation
	std::vector<Entity_Ptr> m_visibles;
	std::vector<Entity_Ptr> m_yokeables;
	Entity_Ptr m_viewer;

	//#################### CONSTRUCTORS ####################
public:
	EntityManager(const std::vector<Entity_Ptr>& entities, const std::vector<AABB3d>& aabbs, const std::string& entDefFilename);

	//#################### PUBLIC METHODS ####################
public:
	const std::vector<AABB3d>& aabbs() const;
	const std::string& definition_filename() const;
	const std::vector<Entity_Ptr>& entities() const;
	const std::vector<Entity_Ptr>& simulables() const;
	Entity_Ptr viewer() const;
	const std::vector<Entity_Ptr>& visibles() const;
	const std::vector<Entity_Ptr>& yokeables() const;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityManager> EntityManager_Ptr;
typedef shared_ptr<const EntityManager> EntityManager_CPtr;

}

#endif

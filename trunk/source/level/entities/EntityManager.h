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
	std::vector<Entity_Ptr> m_visibles;
	std::vector<Entity_Ptr> m_yokeables;
	Entity_Ptr m_player;

	//#################### CONSTRUCTORS ####################
public:
	EntityManager(std::istream& is, const boost::filesystem::path& settingsDir);

	//#################### PUBLIC METHODS ####################
public:
	const AABB3d& aabb(int n) const;
	void save(std::ostream& os) const;
	Entity_Ptr player() const;
	const std::vector<Entity_Ptr>& visibles() const;
	const std::vector<Entity_Ptr>& yokeables() const;

	//#################### PRIVATE METHODS ####################
private:
	void load_entity(std::istream& is);
	static void skip_entity(std::istream& is, const std::string& entityClass);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityManager> EntityManager_Ptr;
typedef shared_ptr<const EntityManager> EntityManager_CPtr;

}

#endif

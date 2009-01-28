/***
 * hesperus: EntityExManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYEXMANAGER
#define H_HESP_ENTITYEXMANAGER

#include <string>
#include <vector>

#include <boost/filesystem/operations.hpp>

#include <source/math/geom/AABB.h>
#include "EntityEx.h"

namespace hesp {

class EntityExManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_entDefFilename;

	std::vector<AABB3d> m_aabbs;
	std::vector<EntityEx_Ptr> m_entities;
	std::vector<EntityEx_Ptr> m_visibles;
	std::vector<EntityEx_Ptr> m_yokeables;
	EntityEx_Ptr m_player;

	//#################### CONSTRUCTORS ####################
public:
	EntityExManager(std::istream& is, const boost::filesystem::path& settingsDir);

	//#################### PUBLIC METHODS ####################
public:
	const AABB3d& aabb(int n) const;
	void save(std::ostream& os) const;
	EntityEx_Ptr player() const;
	const std::vector<EntityEx_Ptr>& visibles() const;
	const std::vector<EntityEx_Ptr>& yokeables() const;

	//#################### PRIVATE METHODS ####################
private:
	void load_entity(std::istream& is);
	static void skip_entity(std::istream& is, const std::string& archetype);
};

}

#endif

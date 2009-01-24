/***
 * hesperus: EntityManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYMANAGER
#define H_HESP_ENTITYMANAGER

#include <vector>

#include <boost/shared_ptr.hpp>
#include <boost/filesystem/operations.hpp>
using boost::shared_ptr;

#include <source/level/entities/Player.h>
#include <source/math/geom/AABB.h>

namespace hesp {

class EntityManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::string m_entDefFilename;

	std::vector<AABB3d> m_aabbs;
	Player_Ptr m_player;

	//#################### CONSTRUCTORS ####################
public:
	EntityManager(std::istream& is, const boost::filesystem::path& settingsDir);

	//#################### PUBLIC METHODS ####################
public:
	void output(std::ostream& os);
	Player_Ptr player() const;

	//#################### PRIVATE METHODS ####################
private:
	void load_entity(std::istream& is);
	static Player_Ptr load_player(std::istream& is);
	static void skip_entity(std::istream& is, const std::string& entityClass);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityManager> EntityManager_Ptr;
typedef shared_ptr<const EntityManager> EntityManager_CPtr;

}

#endif

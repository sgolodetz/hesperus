/***
 * hesperus: EntityManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYMANAGER
#define H_HESP_ENTITYMANAGER

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/entities/Player.h>
#include <source/math/geom/AABB.h>

namespace hesp {

class EntityManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<AABB3d> m_aabbs;
	Player_Ptr m_player;

	//#################### CONSTRUCTORS ####################
public:
	EntityManager(const std::vector<AABB3d>& aabbs);

	//#################### PUBLIC METHODS ####################
public:
	void load_entity(std::istream& is);
	Player_Ptr player() const;

	//#################### PRIVATE METHODS ####################
private:
	static Player_Ptr load_player(std::istream& is);
	static void read_checked_line(std::istream& is, const std::string& expected);
	static std::string read_field(std::istream& is, const std::string& expectedFieldName);
	static std::vector<int> read_intarray_field(std::istream& is, const std::string& expectedFieldName);
	static void read_line(std::istream& is, std::string& line, const std::string& description);
	template <typename T> static T read_typed_field(std::istream& is, const std::string& expectedFieldName);
	static void skip_entity(std::istream& is, const std::string& entityClass);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityManager> EntityManager_Ptr;
typedef shared_ptr<const EntityManager> EntityManager_CPtr;

}

#include "EntityManager.tpp"

#endif

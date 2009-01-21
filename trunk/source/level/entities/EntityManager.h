/***
 * hesperus: EntityManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYMANAGER
#define H_HESP_ENTITYMANAGER

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/AABB.h>

namespace hesp {

class EntityManager
{
	//#################### PRIVATE VARIABLES ####################
private:
	std::vector<AABB3d> m_aabbs;

	//#################### CONSTRUCTORS ####################
public:
	EntityManager(const std::vector<AABB3d>& aabbs);

	//#################### PUBLIC METHODS ####################
public:
	void load_entity(std::istream& is);
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityManager> EntityManager_Ptr;
typedef shared_ptr<const EntityManager> EntityManager_CPtr;

}

#endif

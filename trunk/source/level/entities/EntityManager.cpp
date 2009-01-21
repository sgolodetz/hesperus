/***
 * hesperus: EntityManager.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "EntityManager.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
EntityManager::EntityManager(const std::vector<AABB3d>& aabbs)
:	m_aabbs(aabbs)
{}

//#################### PUBLIC METHODS ####################
void EntityManager::load_entity(std::istream& is)
{
	// NYI
	throw 23;
}

}

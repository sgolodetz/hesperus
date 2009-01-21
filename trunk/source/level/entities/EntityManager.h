/***
 * hesperus: EntityManager.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYMANAGER
#define H_HESP_ENTITYMANAGER

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

namespace hesp {

class EntityManager
{
	// TODO
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityManager> EntityManager_Ptr;
typedef shared_ptr<const EntityManager> EntityManager_CPtr;

}

#endif

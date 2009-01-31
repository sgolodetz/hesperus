/***
 * hesperus: EntityCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_ENTITYCOMMAND
#define H_HESP_ENTITYCOMMAND

#include <vector>

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/math/geom/AABB.h>
#include <source/level/onionbsp/OnionTree.h>

namespace hesp {

class EntityCommand
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~EntityCommand() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void execute(const std::vector<AABB3d>& aabbs, const OnionTree_Ptr& tree, int milliseconds) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityCommand> EntityCommand_Ptr;

}

#endif

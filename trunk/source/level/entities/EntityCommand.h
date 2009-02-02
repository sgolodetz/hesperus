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
#include <source/level/nav/NavDataset.h>

namespace hesp {

class EntityCommand
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~EntityCommand() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void execute(const std::vector<AABB3d>& aabbs, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<EntityCommand> EntityCommand_Ptr;

}

#endif

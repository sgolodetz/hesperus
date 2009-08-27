/***
 * hesperus: ObjectCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTCOMMAND
#define H_HESP_OBJECTCOMMAND

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/nav/NavDataset.h>
#include <source/level/trees/OnionTree.h>
#include "ObjectManager.h"

namespace hesp {

class ObjectCommand
{
	//#################### DESTRUCTOR ####################
public:
	virtual ~ObjectCommand() {}

	//#################### PUBLIC ABSTRACT METHODS ####################
public:
	virtual void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds) = 0;
};

//#################### TYPEDEFS ####################
typedef shared_ptr<ObjectCommand> ObjectCommand_Ptr;
typedef shared_ptr<const ObjectCommand> ObjectCommand_CPtr;

}

#endif

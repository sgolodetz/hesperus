/***
 * hesperus: ObjectCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_OBJECTCOMMAND
#define H_HESP_OBJECTCOMMAND

#include <boost/shared_ptr.hpp>
using boost::shared_ptr;

#include <source/level/objects/base/ObjectID.h>
#include <source/util/PolygonTypes.h>

namespace hesp {

//#################### FORWARD DECLARATIONS ####################
typedef shared_ptr<class NavDataset> NavDataset_Ptr;
typedef shared_ptr<class ObjectManager> ObjectManager_Ptr;
typedef shared_ptr<const class OnionTree> OnionTree_CPtr;

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

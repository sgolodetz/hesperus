/***
 * hesperus: CmdBipedSetLook.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedSetLook.h"

#include <source/axes/NUVAxes.h>
#include <source/level/objects/components/ICmpOrientation.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
CmdBipedSetLook::CmdBipedSetLook(const ObjectID& objectID, const Vector3d& look)
:	m_objectID(objectID), m_look(look)
{}

//#################### PUBLIC METHODS ####################
void CmdBipedSetLook::execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree,
							  const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds)
{
	ICmpOrientation_Ptr cmpOrientation = objectManager->get_component(m_objectID, cmpOrientation);
	cmpOrientation->nuv_axes()->set_n(m_look);
}

}

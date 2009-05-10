/***
 * hesperus: CmdBipedTurn.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDTURN
#define H_HESP_CMDBIPEDTURN

#include "ObjectCommand.h"

namespace hesp {

class CmdBipedTurn : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	int m_mouseMotionX, m_mouseMotionY;

	//#################### CONSTRUCTORS ####################
public:
	CmdBipedTurn(const ObjectID& objectID, int mouseMotionX, int mouseMotionY);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds);
};

}

#endif

/***
 * hesperus: CmdBipedMove.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDMOVE
#define H_HESP_CMDBIPEDMOVE

#include "ObjectCommand.h"

namespace hesp {

class CmdBipedMove : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	Vector3d m_dir;
	double m_speed;

	//#################### CONSTRUCTORS ####################
public:
	CmdBipedMove(const ObjectID& objectID, const Vector3d& dir, double speed);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds);
};

}

#endif

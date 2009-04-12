/***
 * hesperus: BipedTurnCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BIPEDTURNCOMMAND
#define H_HESP_BIPEDTURNCOMMAND

#include <source/level/trees/OnionTree.h>
#include "Entity.h"
#include "EntityCommand.h"

namespace hesp {

class BipedTurnCommand : public EntityCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	Entity_Ptr m_biped;
	int m_mouseMotionX, m_mouseMotionY;

	//#################### CONSTRUCTORS ####################
public:
	BipedTurnCommand(const Entity_Ptr& biped, int mouseMotionX, int mouseMotionY);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const std::vector<AABB3d>& aabbs, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds);
};

}

#endif

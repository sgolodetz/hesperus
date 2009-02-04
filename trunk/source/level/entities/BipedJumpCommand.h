/***
 * hesperus: BipedJumpCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BIPEDJUMPCOMMAND
#define H_HESP_BIPEDJUMPCOMMAND

#include <source/math/vectors/Vector3.h>
#include "Entity.h"
#include "EntityCommand.h"

namespace hesp {

class BipedJumpCommand : public EntityCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	Entity_Ptr m_biped;
	Vector3d m_dir;

	//#################### CONSTRUCTORS ####################
public:
	BipedJumpCommand(const Entity_Ptr& biped, const Vector3d& dir);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const std::vector<AABB3d>& aabbs, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds);
};

}

#endif

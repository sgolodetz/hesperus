/***
 * hesperus: BipedChangePoseCommand.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_BIPEDCHANGEPOSECOMMAND
#define H_HESP_BIPEDCHANGEPOSECOMMAND

#include "Entity.h"
#include "EntityCommand.h"

namespace hesp {

class BipedChangePoseCommand : public EntityCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	Entity_Ptr m_biped;

	//#################### CONSTRUCTORS ####################
public:
	BipedChangePoseCommand(const Entity_Ptr& biped);

	//#################### PUBLIC METHODS ####################
public:
	void execute(const std::vector<AABB3d>& aabbs, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds);
};

}

#endif

/***
 * hesperus: CmdBipedMove.h
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#ifndef H_HESP_CMDBIPEDMOVE
#define H_HESP_CMDBIPEDMOVE

#include <source/level/objects/base/ObjectCommand.h>
#include <source/level/objects/components/ICmpBipedAnimChooser.h>
#include <source/level/objects/components/ICmpMeshMovement.h>

namespace hesp {

class CmdBipedMove : public ObjectCommand
{
	//#################### PRIVATE VARIABLES ####################
private:
	ObjectID m_objectID;
	Vector3d m_dir;

	//#################### CONSTRUCTORS ####################
public:
	CmdBipedMove(const ObjectID& objectID, const Vector3d& dir);

	//#################### PRIVATE ABSTRACT METHODS ####################
private:
	virtual double lookup_speed(const ICmpMeshMovement_CPtr& cmpMovement) const = 0;
	virtual void set_anim_flag(const ICmpBipedAnimChooser_Ptr& cmpAnimChooser) const = 0;

	//#################### PUBLIC METHODS ####################
public:
	void execute(const ObjectManager_Ptr& objectManager, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree, const std::vector<NavDataset_Ptr>& navDatasets, int milliseconds);
};

}

#endif

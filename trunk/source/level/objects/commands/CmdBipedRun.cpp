/***
 * hesperus: CmdBipedRun.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmdBipedRun.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmdBipedRun::CmdBipedRun(const ObjectID& objectID, const Vector3d& dir)
:	CmdBipedMove(objectID, dir)
{}

//#################### PRIVATE METHODS ####################
double CmdBipedRun::lookup_speed(const ICmpMeshMovement_Ptr& cmpMovement) const
{
	return cmpMovement->run_speed();
}

void CmdBipedRun::set_anim_flag(const ICmpBipedAnimChooser_Ptr& cmpAnimChooser) const
{
	cmpAnimChooser->set_run_flag();
}

}

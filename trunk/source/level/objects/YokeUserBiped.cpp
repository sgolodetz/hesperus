/***
 * hesperus: YokeUserBiped.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "YokeUserBiped.h"

#include <source/math/Constants.h>
#include "CmdBipedChangePose.h"
#include "CmdBipedJump.h"
#include "CmdBipedMove.h"
#include "CmdBipedTurn.h"
#include "ICmpMeshMovement.h"
#include "ICmpOrientation.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
YokeUserBiped::YokeUserBiped(const ObjectID& objectID, ObjectManager *objectManager)
:	m_objectID(objectID), m_objectManager(objectManager)
{}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> YokeUserBiped::generate_commands(UserInput& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_Ptr& tree,
																const std::vector<NavDataset_Ptr>& navDatasets)
{
	// FIXME: The key mappings should be defined externally, not hard-coded like this.

	std::vector<ObjectCommand_Ptr> commands;

	ICmpMeshMovement_Ptr cmpMovement = m_objectManager->get_component(m_objectID, cmpMovement);			assert(cmpMovement != NULL);
	ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(m_objectID, cmpOrientation);	assert(cmpOrientation != NULL);

	NUVAxes_Ptr nuvAxes = cmpOrientation->nuv_axes();

	//~~~~~~~~~~~~~~~~
	// NORMAL MOVEMENT
	//~~~~~~~~~~~~~~~~

	Vector3d dir(0,0,0);

	// Work out the forward direction of the biped. Note that we must ensure elsewhere that the
	// look vector is never pointing directly upwards for this to work.
	Vector3d forward = nuvAxes->n();
	forward.z = 0;
	forward.normalize();

	Vector3d right = forward.cross(Vector3d(0,0,1)).normalize();

	if(input.key_down(SDLK_w)) dir += forward;
	if(input.key_down(SDLK_s)) dir -= forward;
	if(input.key_down(SDLK_d)) dir += right;
	if(input.key_down(SDLK_a)) dir -= right;

	if(dir.length() >= SMALL_EPSILON)
	{
		// Prevent faster movement when strafing.
		dir.normalize();

		// Decide whether to run or walk.
		double speed = input.key_down(SDLK_LSHIFT) ? cmpMovement->walk_speed() : cmpMovement->run_speed();

		commands.push_back(ObjectCommand_Ptr(new CmdBipedMove(m_objectID, dir, speed)));
	}

	//~~~~~~~~~~~
	// MOUSE LOOK
	//~~~~~~~~~~~

	if(input.mouse_motion_x() || input.mouse_motion_y())
	{
		commands.push_back(ObjectCommand_Ptr(new CmdBipedTurn(m_objectID, input.mouse_motion_x(), input.mouse_motion_y())));
	}

	//~~~~~~~
	// CROUCH
	//~~~~~~~

	if(input.key_down(SDLK_c))
	{
		commands.push_back(ObjectCommand_Ptr(new CmdBipedChangePose(m_objectID)));
		input.release_key(SDLK_c);
	}

	//~~~~~
	// JUMP
	//~~~~~

	if(input.key_down(SDLK_SPACE))
	{
		commands.push_back(ObjectCommand_Ptr(new CmdBipedJump(m_objectID, dir)));
		input.release_key(SDLK_SPACE);
	}

	return commands;
}

}

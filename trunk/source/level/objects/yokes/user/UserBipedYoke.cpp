/***
 * hesperus: UserBipedYoke.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "UserBipedYoke.h"

#include <source/axes/NUVAxes.h>
#include <source/input/InputBinding.h>
#include <source/input/InputState.h>
#include <source/input/KeyInputter.h>
#include <source/input/MouseButtonInputter.h>
#include <source/io/files/BindingFile.h>
#include <source/io/util/DirectoryFinder.h>
#include <source/level/objects/commands/CmdBipedChangePosture.h>
#include <source/level/objects/commands/CmdBipedJump.h>
#include <source/level/objects/commands/CmdBipedRun.h>
#include <source/level/objects/commands/CmdBipedTurn.h>
#include <source/level/objects/commands/CmdBipedWalk.h>
#include <source/level/objects/commands/CmdUseActiveItem.h>
#include <source/level/objects/components/ICmpOrientation.h>
#include <source/math/Constants.h>
#include <source/util/ConfigOptions.h>

namespace hesp {

//#################### CONSTRUCTORS ####################
UserBipedYoke::UserBipedYoke(const ObjectID& objectID, ObjectManager *objectManager)
:	m_objectID(objectID), m_objectManager(objectManager)
{
	// Look up the user profile in use and determine the location of the binding file.
	std::string profile = ConfigOptions::instance().get<std::string>("profile");
	std::string bindingFilename = (determine_profiles_directory() / profile / "binding.xml").file_string();

	// Load the binding.
	m_inputBinding = BindingFile::load(bindingFilename);
}

//#################### PUBLIC METHODS ####################
std::vector<ObjectCommand_Ptr> UserBipedYoke::generate_commands(InputState& input, const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree,
																const NavManager_CPtr& navManager)
{
	std::vector<ObjectCommand_Ptr> commands;

	ICmpOrientation_Ptr cmpOrientation = m_objectManager->get_component(m_objectID, cmpOrientation);	assert(cmpOrientation != NULL);

	NUVAxes_CPtr nuvAxes = cmpOrientation->nuv_axes();

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

	if(m_inputBinding->down(ACT_MOVE_FORWARD, input))	dir += forward;
	if(m_inputBinding->down(ACT_MOVE_BACKWARD, input))	dir -= forward;
	if(m_inputBinding->down(ACT_STRAFE_RIGHT, input))	dir += right;
	if(m_inputBinding->down(ACT_STRAFE_LEFT, input))	dir -= right;

	if(dir.length() >= SMALL_EPSILON)
	{
		// Prevent faster movement when strafing.
		dir.normalize();

		// Either run or walk, depending on the input.
		if(m_inputBinding->down(ACT_WALK, input))	commands.push_back(ObjectCommand_Ptr(new CmdBipedWalk(m_objectID, dir)));
		else										commands.push_back(ObjectCommand_Ptr(new CmdBipedRun(m_objectID, dir)));
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

	Inputter_CPtr crouchInputter = (*m_inputBinding)(ACT_CROUCH);
	if(crouchInputter && crouchInputter->down(input))
	{
		commands.push_back(ObjectCommand_Ptr(new CmdBipedChangePosture(m_objectID)));
		crouchInputter->release(input);
	}

	//~~~~~
	// JUMP
	//~~~~~

	Inputter_CPtr jumpInputter = (*m_inputBinding)(ACT_JUMP);
	if(jumpInputter && jumpInputter->down(input))
	{
		commands.push_back(ObjectCommand_Ptr(new CmdBipedJump(m_objectID, dir)));
		jumpInputter->release(input);
	}

	//~~~~~~~~~~~
	// ITEM USAGE
	//~~~~~~~~~~~

	Inputter_CPtr useItemInputter = (*m_inputBinding)(ACT_USE_ITEM);
	if(useItemInputter && useItemInputter->down(input))
	{
		commands.push_back(ObjectCommand_Ptr(new CmdUseActiveItem(m_objectID)));
	}

	return commands;
}

}

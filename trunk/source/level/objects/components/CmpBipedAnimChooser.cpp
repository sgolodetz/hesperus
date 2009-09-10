/***
 * hesperus: CmpBipedAnimChooser.cpp
 * Copyright Stuart Golodetz, 2009. All rights reserved.
 ***/

#include "CmpBipedAnimChooser.h"

#include <source/level/nav/NavDataset.h>
#include <source/level/objects/MoveFunctions.h>
#include <source/util/Properties.h>
#include "ICmpAABBBounds.h"
#include "ICmpInventory.h"
#include "ICmpMeshMovement.h"
#include "ICmpOwnable.h"
#include "ICmpPosition.h"

namespace hesp {

//#################### CONSTRUCTORS ####################
CmpBipedAnimChooser::CmpBipedAnimChooser()
{
	reset_flags();
}

//#################### STATIC FACTORY METHODS ####################
IObjectComponent_Ptr CmpBipedAnimChooser::load(const Properties&)
{
	return IObjectComponent_Ptr(new CmpBipedAnimChooser);
}

//#################### PUBLIC METHODS ####################
void CmpBipedAnimChooser::check_dependencies() const
{
	check_dependency<ICmpAABBBounds>();
	check_dependency<ICmpHealth>();
	check_dependency<ICmpMeshMovement>();
	check_dependency<ICmpPosition>();
}

std::string CmpBipedAnimChooser::choose_animation(const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree,
												  const std::vector<NavDataset_Ptr>& navDatasets)
{
	// Determine the biped's alive/dying/dead status. If dead, early-out.
	ICmpHealth::HealthStatus healthStatus = determine_health_status();
	if(healthStatus == ICmpHealth::DEAD) return "dead";

	// If the biped's not yet dead, determine its movement type.
	MovementType movementType = determine_movement_type(polygons, tree, navDatasets);

	// Determine whether or not the biped is crouching.
	bool crouching = determine_crouching();

	// Lookup the current animation extension (if any).
	std::string animExtension = determine_anim_extension();

	// Choose the animation based on movement type, health status, crouch status and animation extension.
	std::string animationName;

	if(healthStatus == ICmpHealth::ALIVE)
	{
		switch(movementType)
		{
			case AIR:	animationName = "air";	break;
			case IDLE:	animationName = "idle";	break;
			case RUN:	animationName = "run";	break;
			case WALK:	animationName = "walk";	break;
			default:	throw Exception("Unexpected movement type");
		}
	}
	else
	{
		if(movementType == AIR)	animationName = "dieair";
		else					animationName = "die";
	}

	if(crouching)			animationName += "crouch";
	if(animExtension != "")	animationName += "_" + animExtension;

	// Reset any flags to false ready for the next frame.
	reset_flags();

	return animationName;
}

Properties CmpBipedAnimChooser::save() const
{
	return Properties();
}

void CmpBipedAnimChooser::set_run_flag()
{
	m_runFlag = true;
}

void CmpBipedAnimChooser::set_walk_flag()
{
	m_walkFlag = true;
}

//#################### PRIVATE METHODS ####################
std::string CmpBipedAnimChooser::determine_anim_extension() const
{
	std::string animExtension;
	ICmpInventory_Ptr cmpInventory = m_objectManager->get_component(m_objectID, cmpInventory);
	if(cmpInventory)
	{
		ObjectID activeItem = cmpInventory->active_item();
		if(activeItem.valid())
		{
			ICmpOwnable_Ptr cmpItemOwnable = m_objectManager->get_component(activeItem, cmpItemOwnable);	assert(cmpItemOwnable);
			animExtension = cmpItemOwnable->anim_extension();
		}
	}
	return animExtension;
}

bool CmpBipedAnimChooser::determine_crouching() const
{
	ICmpAABBBounds_Ptr cmpBounds = m_objectManager->get_component(m_objectID, cmpBounds);	assert(cmpBounds != NULL);

	// FIXME: This only works for bipeds using AABBs 0 and 1.
	return cmpBounds->pose() == 1;
}

ICmpHealth::HealthStatus CmpBipedAnimChooser::determine_health_status() const
{
	ICmpHealth_Ptr cmpHealth = m_objectManager->get_component(m_objectID, cmpHealth);		assert(cmpHealth != NULL);
	return cmpHealth->status();
}

CmpBipedAnimChooser::MovementType CmpBipedAnimChooser::determine_movement_type(const std::vector<CollisionPolygon_Ptr>& polygons, const OnionTree_CPtr& tree,
																			   const std::vector<NavDataset_Ptr>& navDatasets)
{
	MovementType movementType = UNKNOWN;

	// Determine whether or not the biped's in the air.
	ICmpAABBBounds_Ptr cmpBounds = m_objectManager->get_component(m_objectID, cmpBounds);	assert(cmpBounds != NULL);
	if(!MoveFunctions::attempt_navmesh_acquisition(m_objectID, m_objectManager, polygons, tree, navDatasets[cmpBounds->cur_aabb_index()]->nav_mesh()))
	{
		movementType = AIR;
	}
	else
	{
		// If it's not in the air, work out its actual movement type from the flags.
		if(m_runFlag)		movementType = RUN;
		else if(m_walkFlag)	movementType = WALK;
		else				movementType = IDLE;
	}

	return movementType;
}

void CmpBipedAnimChooser::reset_flags()
{
	m_runFlag = m_walkFlag = false;
}

}
